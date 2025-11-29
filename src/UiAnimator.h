#pragma once
#include <QObject>
#include <QWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>

class UiAnimator : public QObject {
    Q_OBJECT
public:
    static void animateHover(QWidget *widget, bool enter) {
        if (!widget) return;

        QPropertyAnimation *anim = new QPropertyAnimation(widget, "geometry");
        QRect rect = widget->geometry();

        if (enter) {
             // Slight scale up logic would require moving geometry, which is tricky in layouts.
             // Instead, let's animate opacity or a shadow if possible.
             // For simplicity in standard widgets: Animate opacity of an effect?
             // Or just let CSS handle hover for color.
             // Let's do a "fade in" effect on show.
        }
    }

    static void fadeIn(QWidget *widget, int duration = 500) {
        if (!widget) return;
        QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(widget);
        widget->setGraphicsEffect(eff);
        QPropertyAnimation *a = new QPropertyAnimation(eff, "opacity");
        a->setDuration(duration);
        a->setStartValue(0);
        a->setEndValue(1);
        a->setEasingCurve(QEasingCurve::OutCubic);
        a->start(QAbstractAnimation::DeleteWhenStopped);
    }

    static void slideIn(QWidget *widget, int duration = 300) {
        if (!widget) return;
        QRect end = widget->geometry();
        QRect start = end.translated(0, 20); // From bottom

        widget->setGeometry(start);
        QPropertyAnimation *a = new QPropertyAnimation(widget, "geometry");
        a->setDuration(duration);
        a->setStartValue(start);
        a->setEndValue(end);
        a->setEasingCurve(QEasingCurve::OutBack);
        a->start(QAbstractAnimation::DeleteWhenStopped);
    }
};
