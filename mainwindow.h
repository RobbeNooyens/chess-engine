#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "game.h"
#include "SchaakStuk.h"
#include <QCloseEvent>
class ChessBoard;
class QSettings;

struct VisualOptions{
    bool moves;
    bool threats;
    bool threatenedPieces;
    VisualOptions(bool m, bool t, bool tP): moves(m), threats(t), threatenedPieces(tP){}
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    void update();
    const VisualOptions getOptions() {return options;}

private:
    ChessBoard *scene;
    Game g;
    VisualOptions options = VisualOptions(false, false, false);
private slots:
    void clicked(int x, int y);
    void newGame();
    void open();
    void save();
    void undo();
    void redo();
    void visualizationChange();


    // De volgende lijnen kunnen genegeerd worden voor de opdracht

    void on_actionExit_triggered();
    void closeEvent (QCloseEvent *event)
    {
        on_actionExit_triggered();
        event->ignore();
    }

private:
    void createActions();
    void createMenus();
    QMenu *fileMenu;
    QMenu *gameMenu;
    QMenu *visualizeMenu;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *exitAct;

    QAction* display_moves;
    QAction* display_kills;
    QAction* display_threats;
};

#endif // MAINWINDOW_H
