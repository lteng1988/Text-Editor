#include "notepad.h"
#include "ui_notepad.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QtWidgets>

//Notepad::Notepad(QWidget *parent) :
//    QMainWindow(parent),
//    ui(new Ui::Notepad)
//{
//    ui->setupUi(this);
//}
Notepad::Notepad()
{
    textEdit = new QPlainTextEdit;
    setCentralWidget(textEdit);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    readSettings();

    connect(textEdit->document(), SIGNAL(contentsChanged()),
            this, SLOT(documentWasModified()));
    setCurrentFile("");
    setUnifiedTitleAndToolBarOnMac(true);
//    saveUserCredentialsForGoogleDrive();

}

Notepad::~Notepad()
{
    //delete ui;
}

/**
 * @brief Notepad::on_pushButton_clicked
 * Define button function
 */
//void Notepad::on_pushButton_clicked()
//{
//    qApp->quit();
//}

//void Notepad::on_actionOpen_triggered()
//{
//    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("Text Files (*.txt);; C++ Files (*.cpp *.h)"));
//    Notepad::currentFileName = fileName;

//    if (!fileName.isEmpty()){
//        QFile file(fileName);
//        if (!file.open(QIODevice::ReadOnly)){
//            QMessageBox::critical(this, tr("Error"), tr("Cloud not open file. Read Only"));
//            return;
//        }

//        QTextStream in(&file);
//        ui->textEdit->setText(in.readAll());
//        file.close(); //Close file
//    }
//}

//void Notepad::write_to_file(QString fileName)
//{
//    if (!fileName.isEmpty()){
//        QFile file(fileName);
//        if (!file.open(QIODevice::WriteOnly)){
//            // Error message
//        } else {
//            QTextStream stream(&file);
//            stream << ui->textEdit->toPlainText();
//            stream.flush();
//            file.close();
//        }
//    }

//}

//void Notepad::on_actionSave_triggered()
//{
//    QString fileName = Notepad::currentFileName;

//    Notepad::write_to_file(fileName);
//}

//void Notepad::on_actionSave_As_triggered()
//{
//    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(), tr("Text Files (*.txt);; C++ Files(*.cpp *.h)"));

//    Notepad::write_to_file(fileName);
//}

void Notepad::newFile()
{
    if (maybeSave()){ //If file was modify
        textEdit->clear();
        setCurrentFile("");
    }
}

void Notepad::open(){
    if (maybeSave()){
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty()){
            loadFile(fileName);
        }
    }

}

bool Notepad::save(){
    if (curFile.isEmpty()){
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool Notepad::saveAs(){
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    QStringList files;
    if (dialog.exec()){
        files = dialog.selectedFiles();
    }else{
        return false;
    }
    return saveFile(files.at(0));

}

void Notepad::documentWasModified(){
    setWindowModified(textEdit->document()->isModified());
}

void Notepad::createActions(){
    newAct = new QAction(QIcon(":/images/new.png"), tr("&new"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(QIcon(":/images/open.png"), tr("&open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open a file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save file"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("&save"), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save as new file"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(tr("&exit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit my awesome notepad"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    cutAct = new QAction(QIcon(":/images/cut.png"), tr("&cut"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut selected text to clipboard"));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    copyAct = new QAction(QIcon(":/images/copy.png"), tr("&copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy selected text to clipboard"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/images/paste.png"),tr("&paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste text from clipboard"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    connect(textEdit, SIGNAL(copyAvailable(bool)),
            cutAct, SLOT(setEnabled(bool)));
    connect(textEdit, SIGNAL(copyAvailable(bool)),
            copyAct, SLOT(setEnabled(bool)));

    saveCredAct = new QAction(tr("&Google Drive Credentials"), this);
    saveCredAct->setStatusTip(tr("Enter Google Drive Credentials"));
    connect(saveCredAct, SIGNAL(triggered()), this, SLOT(saveUserCredentialsForGoogleDrive()));


}

void Notepad::createMenus(){
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);

    menuBar()->addSeparator();

    saveCredMenu = menuBar()->addMenu(tr("&Upload"));
    saveCredMenu->addAction(saveCredAct);


//    helpMenu = menuBar()->addMenu(tr("*&hHelp"));
//    helpMenu->addAction()
}

void Notepad::createToolBars(){
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);


}

void Notepad::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));

}

void Notepad::readSettings(){
    QSettings settings("QtProject", "Application");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}

void Notepad::writeSettings(){
    QSettings settings("QtProject", "Application");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

bool Notepad::maybeSave(){
    if (textEdit->document()->isModified()){
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                                   tr("The document has been modified. \n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard |
                                   QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;

    }
    return true;
}

void Notepad::loadFile(const QString &fileName){
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)){
    QMessageBox::warning(this, tr("Application"),
                         tr("Cannot read file %1:\n%2")
                         .arg(fileName)
                         .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File Loaded"));

}

bool Notepad::saveFile(const QString &fileName){
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, tr("Application"),
                         tr("Cannot write to file %1:\n%2.")
                            .arg(fileName)
                            .arg(file.errorString()));
        return false;

    }

    QTextStream out(&file);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << textEdit->toPlainText();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File Save"));
    return true;
}
void Notepad::setCurrentFile(const QString &fileName){
    curFile = fileName;
    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitiled.txt";
    setWindowFilePath(shownName);

}

QString Notepad::strippedName(const QString &fullFileName){
    return QFileInfo(fullFileName).fileName();
}

bool Notepad::uploadFile(){
//    QNetworkRequest request(QUrl("www.googleapis.com/upload/drive/v2/files?uploadType=media"));
//    request.setHeader(QNetworkRequest::ContentTypeHeader,"text");

//    QByteArray data;
//    QUrl params;

//    params.addQueryItem("userid","user");
//    params.addQueryItem("apiKey","key");
//    data.append(params.toString());
//    data.remove(0,1);

//    QNetworkReply *reply = nwam.post(request,data);
//    return reply;
    return true;
}

bool Notepad::saveUserCredentialsForGoogleDrive(){




    QDialog *dlgMultiLine = new QDialog(this);
    QGridLayout *gridLayout = new QGridLayout(dlgMultiLine);
    QPlainTextEdit *txtMultiline = new QPlainTextEdit;

    txtMultiline->setObjectName(QString::fromUtf8("txtMultiline"));
    QLabel *login = new QLabel(tr("Login"));

    gridLayout->addWidget(login, 0, 0, 1, 1);

    QLineEdit *loginInput = new QLineEdit();

    gridLayout->addWidget(loginInput, 0, 1, 1, 1);

    QLabel *password = new QLabel(tr("Password"));

    gridLayout->addWidget(password, 1, 0, 1, 1);

    QLineEdit *passwordInput = new QLineEdit();
    passwordInput->setEchoMode(QLineEdit::Password);

    gridLayout->addWidget(passwordInput, 1, 1, 1, 1);
//    gridLayout->addWidget(txtMultiline, 0, 0, 1, 1);


    QDialogButtonBox *buttonBox = new QDialogButtonBox();
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    gridLayout->addWidget(buttonBox, 2, 0, 1, 1);

    connect(buttonBox,SIGNAL(accepted()),dlgMultiLine,SLOT(accept()));
    connect(buttonBox,SIGNAL(rejected()),dlgMultiLine, SLOT(reject()));

    if (dlgMultiLine->exec() == QDialog::Accepted) {
        // If the user didn't dismiss the dialog, do something with the fields
        qDebug() << loginInput->text();
        qDebug() << passwordInput->text();
    }else if (dlgMultiLine->exec() == QDialog::Rejected){

    }

//    bool ok;
//    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
//                                         tr("Login: "), QLineEdit::Normal,
//                                         "", &ok);
//    if (ok && !text.isEmpty()){
//        QString a;
//        a = textEdit->toPlainText() + text;
//        textEdit->setPlainText(a);
//    }
    return true;
}
