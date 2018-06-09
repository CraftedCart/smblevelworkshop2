#include "ws2editor/widget/CollapsableWidget.hpp"
#include <QPropertyAnimation>

namespace WS2Editor {
    namespace Widget {
        CollapsableWidget::CollapsableWidget(
                const QString &title,
                const int animationDuration,
                QWidget *parent) :
            QWidget(parent),
            animationDuration(animationDuration)
        {
            //toggleButton.setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            //toggleButton.setArrowType(Qt::ArrowType::RightArrow);
            toggleButton.setText(title);
            toggleButton.setCheckable(true);
            toggleButton.setChecked(false);
            toggleButton.setObjectName("collapsableWidgetToggleButton");
            toggleButton.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

            contentArea.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            //Start out collapsed
            contentArea.setMaximumHeight(0);
            contentArea.setMinimumHeight(0);
            contentArea.setObjectName("collapsableWidgetContent");

            //Let the entire widget grow and shrink with its content
            toggleAnimation.addAnimation(new QPropertyAnimation(this, "maximumHeight"));
            toggleAnimation.addAnimation(new QPropertyAnimation(&contentArea, "maximumHeight"));

            //Don't waste space
            mainLayout.setSpacing(0);
            mainLayout.setMargin(0);

            mainLayout.addWidget(&toggleButton);
            mainLayout.addWidget(&contentArea);

            setLayout(&mainLayout);

            QObject::connect(&toggleButton, &QToolButton::clicked, this, &CollapsableWidget::toggleContentShown);
        }

        void CollapsableWidget::toggleContentShown(bool visible) {
            //The toggle button may not always be synced up with the visibility (eg: if this function was called in code)
            //So make sure it is
            //Also set the flippy arrow
            toggleButton.setChecked(visible);
            //toggleButton.setArrowType(visible ? Qt::ArrowType::DownArrow : Qt::ArrowType::RightArrow);

            //Fetch the current and target heights
            const int collapsedHeight = sizeHint().height() - contentArea.height();
            const int contentHeight = contentArea.layout()->sizeHint().height();

            //Setup all the animations
            QPropertyAnimation *spoilerAnimation = static_cast<QPropertyAnimation*>(toggleAnimation.animationAt(0));
            spoilerAnimation->setDuration(animationDuration);
            spoilerAnimation->setStartValue(collapsedHeight);
            spoilerAnimation->setEndValue(collapsedHeight + contentHeight);

            //Setup the contentArea maximumHeight animation
            QPropertyAnimation *contentAnimation = static_cast<QPropertyAnimation*>(toggleAnimation.animationAt(1));
            contentAnimation->setDuration(animationDuration);
            contentAnimation->setStartValue(0);
            contentAnimation->setEndValue(contentHeight);

            //Kickstart the animation
            toggleAnimation.setDirection(visible ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
            toggleAnimation.start();
        }

        void CollapsableWidget::setContentLayout(QLayout &contentLayout) {
            delete contentArea.layout();
            contentArea.setLayout(&contentLayout);
        }

    }
}

