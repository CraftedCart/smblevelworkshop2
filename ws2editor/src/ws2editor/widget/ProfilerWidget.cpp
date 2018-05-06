#include "ws2editor/widget/ProfilerWidget.hpp"
#include <QPainter>

namespace WS2Editor {
    namespace Widget {
        using namespace WS2Common::Profile;

        ProfilerWidget::ProfilerWidget(QWidget *parent) : QWidget(parent) {
            setMinimumSize(64, 64);
        }

        void ProfilerWidget::paintEvent(QPaintEvent *event) {
            Q_UNUSED(event);

            QPainter painter(this);

            //Material colors
            static const QColor profilerColors[] = {
                QColor(244, 67, 54),
                QColor(33, 150, 243),
                QColor(139, 195, 74),
                QColor(255, 87, 34),
                QColor(233, 30, 99),
                QColor(3, 169, 244),
                QColor(205, 220, 57),
                QColor(121, 85, 72),
                QColor(156, 39, 176),
                QColor(0, 188, 212),
                QColor(255, 235, 59),
                QColor(158, 158, 158),
                QColor(103, 58, 183),
                QColor(0, 150, 136),
                QColor(255, 193, 7),
                QColor(96, 125, 139),
                QColor(63, 81, 181),
                QColor(76, 175, 80),
                QColor(255, 152, 0)
            };

            QFontMetrics fontMetrics((QFont()));

            //Draw the bar
            qint64 totalNsecs = profiler->getTotalCommittedNanoseconds();
            float nextBarStart = 0.0f; //X position of the next bar to draw
            for (int i = 0; i < profiler->getCommittedSegments().size(); i++) {
                const ProfileSegment *s = profiler->getCommittedSegments().at(i);
                float barWidth = ((float) s->getCommittedNanoseconds() / totalNsecs) * width();
                const QColor &barColor = profilerColors[i % 19]; //Mod 19 because there's 19 colors
                QRectF barRect(nextBarStart, 0.0f, barWidth, 24.0f);

                QLinearGradient gradient(barRect.topLeft(), barRect.bottomLeft());
                gradient.setColorAt(0.5f, palette().color(backgroundRole()));
                gradient.setColorAt(1.0f, barColor);
                painter.fillRect(barRect, gradient);

                painter.drawLine(nextBarStart, 0.0f, nextBarStart, 24.0f);

                painter.drawText(nextBarStart + 4, fontMetrics.height(), s->getName());

                nextBarStart += barWidth;
            }

            //Draw the bar timings below the bar
            painter.drawText(4, 24 + fontMetrics.height(), tr("0 ms"));
            QString endMsecsStr = tr("%1 ms").arg(QString::number(totalNsecs / 1000000.0f, 'f'));
            painter.drawText(width() - 4 - fontMetrics.width(endMsecsStr), 24 + fontMetrics.height(), endMsecsStr);

            //Draw the key for each segment under the bar
            const float keyOffset = 24.0f + 8.0f + fontMetrics.height(); //Bar height + bar timings height + a bit of padding
            for (int i = 0; i < profiler->getCommittedSegments().size(); i++) {
                const ProfileSegment *s = profiler->getCommittedSegments().at(i);
                const QColor &barColor = profilerColors[i % 19]; //Mod 19 because there's 19 colors
                const float yPos = fontMetrics.height() * i + keyOffset;
                QRectF indicatorRect(0.0f, yPos, 8.0f, fontMetrics.height());
                QRectF barRect(8.0f, yPos, width() - 8.0f, fontMetrics.height());

                QLinearGradient gradient(barRect.topLeft(), barRect.bottomLeft());
                gradient.setColorAt(0.7f, palette().color(backgroundRole()));
                gradient.setColorAt(1.0f, barColor);

                painter.fillRect(indicatorRect, barColor);
                painter.fillRect(barRect, gradient);
                painter.drawText(12.0f, yPos + fontMetrics.height() - 4.0f, s->getName());
                QString elapsedTimeStr = tr("%1 ms").arg(QString::number(s->getCommittedNanoseconds() / 1000000.0f, 'f'));
                painter.drawText(width() - 4.0f - fontMetrics.width(elapsedTimeStr), yPos + fontMetrics.height() - 4.0f, elapsedTimeStr);
            }

            setMinimumSize(64, fontMetrics.height() * profiler->getCommittedSegments().size() + keyOffset);

            painter.end();
        }

        void ProfilerWidget::onProfilerNextFrame(Profiler &profiler) {
            this->profiler = &profiler;
            update();
        }
    }
}

