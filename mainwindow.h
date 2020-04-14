#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>

class Parser;
class ParseDialog;
class Finder;
class Loader;
class Calculator;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    
private slots:
    void on_actionAbout_Qt_triggered();
    
    void on_action_About_Dblparse_triggered();
    
    void on_actionAbout_DBLP_triggered();
    
    void on_actionE_xit_triggered();
    
    void on_action_Open_triggered();
    
    void on_action_Status_triggered();
    
    void on_action_Clear_Index_triggered();
    
    void on_action_Open_Index_Folder_triggered();
    
    void on_actionAuthorStac_triggered();

    void on_actionView_Log_triggered();
    
    void on_action_Settings_triggered();
    
    void on_action_Count_Clique_triggered();
    
private:
    Ui::MainWindow *ui;
    Parser *m_parser;
    QThread m_parseThread;
    Finder *m_finder;
    Loader *m_loader;
    Calculator *m_calculator;
    QThread m_calcThread;
    
    void load();
    void calc();
    void handleCalc();
    
signals:
    void startParse();
    void startCalc();
};
#endif // MAINWINDOW_H
