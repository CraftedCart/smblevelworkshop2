#include "ws2lz/LZCompressor.hpp"
#include "ws2lz/LZSSDictionary.hpp"

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {

/**************************************************************
        LZSS.C -- A Data Compression Program
        (tab = 4 spaces)
***************************************************************
        4/6/1989 Haruhiko Okumura
        Use, distribute, and modify this program freely.
        Please send me your improved versions.
                PC-VAN		SCIENCE
                NIFTY-Serve	PAF01022
                CompuServe	74050,1022
**************************************************************
    12/13/2021 camthesaxman
    Changed filler byte to 0 and added header containing sizes
**************************************************************/
#define N 4096 /* size of ring buffer */
#define F 18   /* upper limit for match_length */
#define THRESHOLD                                       \
    2         /* encode string into position and length \
                                     if match_length is greater than this */
#define NIL N /* index for root of binary search trees */

unsigned long int textsize = 0,             /* text size counter */
    codesize = 0,                           /* code size counter */
    printcount = 0;                         /* counter for reporting progress every 1K bytes */
unsigned char text_buf[N + F - 1];          /* ring buffer of size N,
                            with extra F-1 bytes to facilitate string comparison */
int match_position, match_length,           /* of longest match.  These are
                     set by the InsertNode() procedure. */
    lson[N + 1], rson[N + 257], dad[N + 1]; /* left & right children &
           parents -- These constitute binary search trees. */
FILE *infile, *outfile;                     /* input & output files */

void InitTree(void) /* initialize trees */
{
    int i;

    /* For i = 0 to N - 1, rson[i] and lson[i] will be the right and
       left children of node i.  These nodes need not be initialized.
       Also, dad[i] is the parent of node i.  These are initialized to
       NIL (= N), which stands for 'not used.'
       For i = 0 to 255, rson[N + i + 1] is the root of the tree
       for strings that begin with character i.  These are initialized
       to NIL.  Note there are 256 trees. */

    for (i = N + 1; i <= N + 256; i++) rson[i] = NIL;
    for (i = 0; i < N; i++) dad[i] = NIL;
}

void InsertNode(int r)
/* Inserts string of length F, text_buf[r..r+F-1], into one of the
   trees (text_buf[r]'th tree) and returns the longest-match position
   and length via the global variables match_position and match_length.
   If match_length = F, then removes the old node in favor of the new
   one, because the old one will be deleted sooner.
   Note r plays double role, as tree node and position in buffer. */
{
    int i, p, cmp;
    unsigned char* key;

    cmp = 1;
    key = &text_buf[r];
    p = N + 1 + key[0];
    rson[r] = lson[r] = NIL;
    match_length = 0;
    for (;;) {
        if (cmp >= 0) {
            if (rson[p] != NIL)
                p = rson[p];
            else {
                rson[p] = r;
                dad[r] = p;
                return;
            }
        } else {
            if (lson[p] != NIL)
                p = lson[p];
            else {
                lson[p] = r;
                dad[r] = p;
                return;
            }
        }
        for (i = 1; i < F; i++)
            if ((cmp = key[i] - text_buf[p + i]) != 0) break;
        if (i > match_length) {
            match_position = p;
            if ((match_length = i) >= F) break;
        }
    }
    dad[r] = dad[p];
    lson[r] = lson[p];
    rson[r] = rson[p];
    dad[lson[p]] = r;
    dad[rson[p]] = r;
    if (rson[dad[p]] == p)
        rson[dad[p]] = r;
    else
        lson[dad[p]] = r;
    dad[p] = NIL; /* remove p */
}

void DeleteNode(int p) /* deletes node p from tree */
{
    int q;

    if (dad[p] == NIL) return; /* not in tree */
    if (rson[p] == NIL)
        q = lson[p];
    else if (lson[p] == NIL)
        q = rson[p];
    else {
        q = lson[p];
        if (rson[q] != NIL) {
            do {
                q = rson[q];
            } while (rson[q] != NIL);
            rson[dad[q]] = lson[q];
            dad[lson[q]] = dad[q];
            lson[q] = lson[p];
            dad[lson[p]] = q;
        }
        rson[q] = rson[p];
        dad[rson[p]] = q;
    }
    dad[q] = dad[p];
    if (rson[dad[p]] == p)
        rson[dad[p]] = q;
    else
        lson[dad[p]] = q;
    dad[p] = NIL;
}

void Encode(void) {
    int i, c, len, r, s, last_match_length, code_buf_ptr;
    unsigned char code_buf[17], mask;

    InitTree();      /* initialize trees */
    code_buf[0] = 0; /* code_buf[1..16] saves eight units of code, and
           code_buf[0] works as eight flags, "1" representing that the unit
           is an unencoded letter (1 byte), "0" a position-and-length pair
           (2 bytes).  Thus, eight units require at most 16 bytes of code. */
    code_buf_ptr = mask = 1;
    s = 0;
    r = N - F;
    for (i = s; i < r; i++)
        text_buf[i] = 0; /* Clear the buffer with
any character that will appear often. */
    for (len = 0; len < F && (c = getc(infile)) != EOF; len++)
        text_buf[r + len] = c;         /* Read F bytes into the last F bytes of
                       the buffer */
    if ((textsize = len) == 0) return; /* text of size zero */
    for (i = 1; i <= F; i++)
        InsertNode(r - i); /* Insert the F strings,
each of which begins with one or more 'space' characters.  Note
the order in which these strings are inserted.  This way,
degenerate trees will be less likely to occur. */
    InsertNode(r);         /* Finally, insert the whole string just read.  The
                   global variables match_length and match_position are set. */
    do {
        if (match_length > len)
            match_length = len; /* match_length
may be spuriously long near the end of text. */
        if (match_length <= THRESHOLD) {
            match_length = 1;                       /* Not long enough match.  Send one byte. */
            code_buf[0] |= mask;                    /* 'send one byte' flag */
            code_buf[code_buf_ptr++] = text_buf[r]; /* Send uncoded. */
        } else {
            code_buf[code_buf_ptr++] = (unsigned char)match_position;
            code_buf[code_buf_ptr++] =
                (unsigned char)(((match_position >> 4) & 0xf0) |
                                (match_length - (THRESHOLD + 1))); /* Send position and
                                           length pair. Note match_length > THRESHOLD. */
        }
        if ((mask <<= 1) == 0) {               /* Shift mask left one bit. */
            for (i = 0; i < code_buf_ptr; i++) /* Send at most 8 units of */
                putc(code_buf[i], outfile);    /* code together */
            codesize += code_buf_ptr;
            code_buf[0] = 0;
            code_buf_ptr = mask = 1;
        }
        last_match_length = match_length;
        for (i = 0; i < last_match_length && (c = getc(infile)) != EOF; i++) {
            DeleteNode(s);   /* Delete old strings and */
            text_buf[s] = c; /* read new bytes */
            if (s < F - 1)
                text_buf[s + N] = c; /* If the position is
        near the end of buffer, extend the buffer to make
        string comparison easier. */
            s = (s + 1) & (N - 1);
            r = (r + 1) & (N - 1);
            /* Since this is a ring buffer, increment the position
               modulo N. */
            InsertNode(r); /* Register the string in text_buf[r..r+F-1] */
        }
        if ((textsize += i) > printcount) {
            printf("%12ld\r", textsize);
            printcount += 1024;
            /* Reports progress each time the textsize exceeds
               multiples of 1024. */
        }
        while (i++ < last_match_length) { /* After the end of text, */
            DeleteNode(s);                /* no need to read, but */
            s = (s + 1) & (N - 1);
            r = (r + 1) & (N - 1);
            if (--len) InsertNode(r); /* buffer may not be empty. */
        }
    } while (len > 0);      /* until length of string to be processed is zero */
    if (code_buf_ptr > 1) { /* Send remaining code. */
        for (i = 0; i < code_buf_ptr; i++) putc(code_buf[i], outfile);
        codesize += code_buf_ptr;
    }
    printf("In : %ld bytes\n", textsize); /* Encoding is done. */
    printf("Out: %ld bytes\n", codesize);
    printf("Out/In: %.3f\n", (double)codesize / textsize);
}

void Decode(void) /* Just the reverse of Encode(). */
{
    int i, j, k, r, c;
    unsigned int flags;

    for (i = 0; i < N - F; i++) text_buf[i] = 0;
    r = N - F;
    flags = 0;
    for (;;) {
        if (((flags >>= 1) & 256) == 0) {
            if ((c = getc(infile)) == EOF) break;
            flags = c | 0xff00; /* uses higher byte cleverly */
        }                       /* to count eight */
        if (flags & 1) {
            if ((c = getc(infile)) == EOF) break;
            putc(c, outfile);
            text_buf[r++] = c;
            r &= (N - 1);
        } else {
            if ((i = getc(infile)) == EOF) break;
            if ((j = getc(infile)) == EOF) break;
            i |= ((j & 0xf0) << 4);
            j = (j & 0x0f) + THRESHOLD;
            for (k = 0; k <= j; k++) {
                c = text_buf[(i + k) & (N - 1)];
                putc(c, outfile);
                text_buf[r++] = c;
                r &= (N - 1);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    char* s;
    int should_encode;

    if (argc != 4) {
        printf(
            "'lzss e file1 file2' encodes file1 into file2.\n"
            "'lzss d file2 file1' decodes file2 into file1.\n");
        return EXIT_FAILURE;
    }
    if ((s = argv[1], s[1] || strpbrk(s, "DEde") == NULL) ||
        (s = argv[2], (infile = fopen(s, "rb")) == NULL) ||
        (s = argv[3], (outfile = fopen(s, "wb")) == NULL)) {
        printf("??? %s\n", s);
        return EXIT_FAILURE;
    }
    should_encode = toupper(*argv[1]) == 'E';

    if (should_encode) {
        /* leave space for the header */
        fseek(outfile, 8, SEEK_SET);
    } else {
        /* skip over header */
        fseek(infile, 8, SEEK_SET);
    }

    if (should_encode)
        Encode();
    else
        Decode();

    if (should_encode) {
        unsigned char buf[8];
        int i;
        size_t size;
        /* compressed size */
        size = ftell(outfile);
        for (i = 0; i < 4; i++) buf[i] = (size >> (8 * i)) & 0xFF;
        /* original size */
        fseek(infile, 0, SEEK_END);
        size = ftell(infile);
        for (i = 0; i < 4; i++) buf[i + 4] = (size >> (8 * i)) & 0xFF;
        /* write header */
        fseek(outfile, 0, SEEK_SET);
        fwrite(buf, sizeof(buf), 1, outfile);
    }

    fclose(infile);
    fclose(outfile);
    return EXIT_SUCCESS;
}

}

namespace WS2Lz {
    QByteArray LZCompressor::compress(QByteArray data) {
        LZSSDictionary dict;

        // Prime the dictionary
        dict.ptr = dict.WINDOW_SIZE-2*dict.MAX_REF_LEN;

        for (unsigned int i=0; i<dict.MAX_REF_LEN; i++) {
            QByteArray zeroByteList((dict.MAX_REF_LEN-i), '\0');
            QByteArray dataByteList = data.left(i);

            QByteArray byteList(zeroByteList);
            byteList.append(dataByteList);

            dict.add(byteList);
        }

        // Output data
        QByteArray output;
        unsigned int dataSize = data.length();

        unsigned int i = 0;
        while (i < dataSize) {
            QByteArray chunk;
            int flags = 0;

            for (int bit = 0; bit < 8; bit++) {
                if (i >= dataSize) break;

                QByteArray subStr = data.mid(i, qMin(i+dict.MAX_REF_LEN, dataSize));
                offsetLength* ol = dict.find(subStr);

                // Yes, append dictionary reference
                if (ol != nullptr) {
                    chunk.append((char) (ol->first) & 0xFF);
                    chunk.append((char) (((ol->first >> 4) & 0xF0) | (ol->second - dict.MIN_REF_LEN)));

                    for (unsigned int j = 0; j < ol->second; j++) {
                        dict.add(data.mid(i+j, qMin(i+j+dict.MAX_REF_LEN, dataSize)));
                    }

                    i += ol->second;
                }

                // Not in dictionray, append literal
                else {
                    // Append literal value
                    char v = data[i];
                    chunk.append(v);

                    flags |= (1 << bit);

                    // Update dictionary
                    dict.add(data.mid(i, qMin(i+dict.MAX_REF_LEN, dataSize)));
                    i += 1;


                }
            }

            // Chunk complete, add to output
            output.append((char)flags);
            output.append(chunk);
        }

        // Add uncompressed size to the beginning (int, little endian)
        unsigned int uncompressedSize = dataSize;
        output.insert(0, (char) ((uncompressedSize >> 0) & 0xFF));
        output.insert(1, (char) ((uncompressedSize >> 8) & 0xFF));
        output.insert(2, (char) ((uncompressedSize >> 16) & 0xFF));
        output.insert(3, (char) ((uncompressedSize >> 24) & 0xFF));

        // Add compressed size to the beginning (int, little endian - including these new 4 bytes)
        unsigned int compressedSize = output.size() + 4;
        output.insert(0, (char) ((compressedSize >> 0) & 0xFF));
        output.insert(1, (char) ((compressedSize >> 8) & 0xFF));
        output.insert(2, (char) ((compressedSize >> 16) & 0xFF));
        output.insert(3, (char) ((compressedSize >> 24) & 0xFF));

        return output;
    }
}
