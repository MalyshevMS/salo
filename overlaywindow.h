#ifndef OVERLAYWINDOW_H
#define OVERLAYWINDOW_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QTabletEvent>
#include <QMouseEvent>
#include <QKeyEvent>

struct DrawPoint {
    QPointF pos;
    qreal pressure;
    bool isEraser;
};

class OverlayWindow : public QWidget {
    Q_OBJECT

public:
    explicit OverlayWindow(QWidget *parent = nullptr) : QWidget(parent) {
        setAttribute(Qt::WA_TranslucentBackground);
        setAttribute(Qt::WA_NoSystemBackground);
        setMouseTracking(true);
    }

    void clearCanvas() {
        m_canvas.fill(Qt::transparent);
        update();
    }

protected:
    void resizeEvent(QResizeEvent *event) override {
        QImage newCanvas(size(), QImage::Format_ARGB32_Premultiplied);
        newCanvas.fill(Qt::transparent);

        if (!m_canvas.isNull()) {
            QPainter p(&newCanvas);
            p.drawImage(0, 0, m_canvas);
        }
        m_canvas = newCanvas;
        QWidget::resizeEvent(event);
    }

    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.drawImage(0, 0, m_canvas);
    }

    void tabletEvent(QTabletEvent *event) override {
        QPointF pos = event->position();
        qreal pressure = event->pressure();

        if (event->type() == QEvent::TabletPress || event->type() == QEvent::TabletMove) {
            if (event->buttons() & Qt::LeftButton) {
                drawLineTo(pos, pressure, false);
            } else if (event->buttons() & Qt::RightButton || event->pointerType() == QPointingDevice::PointerType::Eraser) {
                drawLineTo(pos, pressure, true);
            }
        } else if (event->type() == QEvent::TabletRelease) {
            m_lastPos = QPointF();
        }

        event->accept();
    }

    void mousePressEvent(QMouseEvent *event) override {
        m_lastPos = event->position();
        handleMouse(event);
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        handleMouse(event);
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        Q_UNUSED(event);
        m_lastPos = QPointF();
    }

    void keyPressEvent(QKeyEvent *event) override {
        if (event->key() == Qt::Key_Escape) {
            hide();
        } else if (event->key() == Qt::Key_C) {
            clearCanvas();
        }
    }

private:
    QImage m_canvas;
    QPointF m_lastPos;

    void handleMouse(QMouseEvent *event) {
        if (event->buttons() & Qt::LeftButton) {
            drawLineTo(event->position(), 1.0, false);
        } else if (event->buttons() & Qt::RightButton) {
            drawLineTo(event->position(), 1.0, true);
        }
    }

    void drawLineTo(const QPointF &endPos, qreal pressure, bool isEraser) {
        if (m_lastPos.isNull()) {
            m_lastPos = endPos;
            return;
        }

        QPainter painter(&m_canvas);
        painter.setRenderHint(QPainter::Antialiasing, true);

        if (isEraser) {
            painter.setCompositionMode(QPainter::CompositionMode_Clear);
            qreal width = 30.0 * (pressure > 0 ? pressure : 1.0);
            QPen pen(Qt::transparent, width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
            painter.setPen(pen);
            painter.drawLine(m_lastPos, endPos);
        } else {
            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
            qreal baseWidth = 5.0;
            qreal width = baseWidth * (0.2 + pressure * 1.8);

            QPen pen(QColor(255, 50, 50, 220), width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
            painter.setPen(pen);
            painter.drawLine(m_lastPos, endPos);
        }

        m_lastPos = endPos;
        update();
    }
};

#endif // OVERLAYWINDOW_H
