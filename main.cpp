#include <QApplication>
#include <QScreen>
#include <QVector>
#include <LayerShellQt/Window>
#include "overlaywindow.h"

int main(int argc, char *argv[]) {
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QApplication app(argc, argv);

    QVector<OverlayWindow*> windows;
    const auto screens = QGuiApplication::screens();

    for (QScreen *screen : screens) {
        auto *window = new OverlayWindow();

        window->winId(); 

        LayerShellQt::Window *layerWindow = LayerShellQt::Window::get(window->windowHandle());
        if (layerWindow) {
            layerWindow->setScreen(screen);

            layerWindow->setLayer(LayerShellQt::Window::LayerOverlay);

            layerWindow->setAnchors(LayerShellQt::Window::Anchors(
                LayerShellQt::Window::AnchorTop |
                LayerShellQt::Window::AnchorBottom |
                LayerShellQt::Window::AnchorLeft |
                LayerShellQt::Window::AnchorRight
            ));

            layerWindow->setExclusiveZone(-1);

            layerWindow->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityExclusive);
        }

        // Показываем окно
        window->show();
        windows.append(window);
    }

    return app.exec();
}
