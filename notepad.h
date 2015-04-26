#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>

class QAction;
class QMenu;
class QPlainTextEdit;

class Notepad : public QMainWindow
{
    Q_OBJECT

public:
    Notepad();
    ~Notepad();

protected:
//    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots: //Slots are functions that listen to events
//    void on_pushButton_clicked();

//    void on_actionOpen_triggered();

//    void on_actionSave_triggered();

//    void on_actionSave_As_triggered();
//    void write_to_file(QString fileName);
    void newFile();
    void open();
    bool save();
    bool saveAs();

    bool saveUserCredentialsForGoogleDrive();

//    void about();
    void documentWasModified();





private:
//    Ui::Notepad *ui;
    QString currentFileName;
    void createActions(); //Create action objects
    void createMenus(); //Create meny objects
    void createToolBars(); //Create tool bars
    void createStatusBar(); //Create status bar
    void readSettings();
    void writeSettings();
    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    bool uploadFile();


    QPlainTextEdit *textEdit; //Predefined plantext ui object
    QString curFile;

    QMenu *fileMenu; //Predefined menu object
    QMenu *editMenu; //
    QMenu *saveCredMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;

    QAction *saveCredAct;
//    QAction *aboutAct;
//    QAction *aboutQtAct;

    QString copyPng;

};

#endif // NOTEPAD_H
