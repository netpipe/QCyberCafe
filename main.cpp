#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPainter>
#include <QKeyEvent>
#include <QProcess>
#include <QTimer>

#ifdef Q_OS_MAC
#include <ApplicationServices/ApplicationServices.h>
#endif

class Locker : public QWidget {
    Q_OBJECT

public:
    Locker() {
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        setWindowState(Qt::WindowFullScreen);

        // Central layout
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setAlignment(Qt::AlignCenter);

        passwordBox = new QLineEdit(this);
        passwordBox->setEchoMode(QLineEdit::Password);
        passwordBox->setPlaceholderText("Enter password...");
        passwordBox->setFixedWidth(300);

        layout->addWidget(passwordBox);
        setLayout(layout);

        connect(passwordBox, &QLineEdit::returnPressed, this, &Locker::checkPassword);

        // Background image
        background = QPixmap(":/background.jpg");

        grabKeyboard();
        grabMouse();

#ifdef Q_OS_MAC
        CGDisplayHideCursor(kCGDirectMainDisplay);
        CGAssociateMouseAndMouseCursorPosition(false);

        hideDockAndMenuBar();
        disableForceQuit();
#endif
    }

    ~Locker() {
#ifdef Q_OS_MAC
        CGDisplayShowCursor(kCGDirectMainDisplay);
        CGAssociateMouseAndMouseCursorPosition(true);

        showDockAndMenuBar();
#endif
    }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.drawPixmap(rect(), background);
    }

    void keyPressEvent(QKeyEvent *event) override {
        // Block common system keys
        if (event->modifiers() & Qt::MetaModifier ||
            event->modifiers() & Qt::AltModifier ||
            event->modifiers() & Qt::ControlModifier) {
            return; // block Cmd, Option, Ctrl combos
        }
        QWidget::keyPressEvent(event);
    }

private slots:
    void checkPassword() {
        if (passwordBox->text() == "letmein") {
            releaseKeyboard();
            releaseMouse();
            close();
        } else {
            passwordBox->clear();
        }
    }

private:
    QLineEdit *passwordBox;
    QPixmap background;

#ifdef Q_OS_MAC
    void hideDockAndMenuBar() {
        QProcess::execute("defaults write com.apple.dock autohide -bool true; killall Dock");
        QProcess::execute("osascript -e 'tell application \"System Events\" to set visible of process \"Finder\" to false'");
    }

    void showDockAndMenuBar() {
        QProcess::execute("defaults write com.apple.dock autohide -bool false; killall Dock");
        QProcess::execute("osascript -e 'tell application \"System Events\" to set visible of process \"Finder\" to true'");
    }

    void disableForceQuit() {
        // This requires Accessibility permissions
        QProcess::startDetached("osascript", {
            "-e",
            R"(
                tell application "System Events"
                    set frontmost of process "loginwindow" to true
                end tell
            )"
        });
    }
#endif
};



int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Locker locker;
    locker.show();
    return a.exec();
}
#include "main.moc"
