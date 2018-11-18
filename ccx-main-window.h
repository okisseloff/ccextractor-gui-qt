#ifndef CCXMAINWINDOW_H
#define CCXMAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QFile>
#include "ccx-options.h"
#include "ccx-about.h"

namespace Ui {
	class CCXMainWindow;
}

class CCXMainWindow : public QMainWindow
{
Q_OBJECT
public:
	explicit CCXMainWindow(QWidget *parent = 0);
	~CCXMainWindow();

private slots:
	void on_leAdditionalOptions_textChanged(const QString &arg1);
	void on_cbVerboseLog_toggled(bool checked);
	void on_twSource_currentChanged(int index);
	void on_leSourceUDPHost_textChanged(const QString &arg1);
	void on_leSourceUDPPort_textChanged(const QString &arg1);
	void on_leSourceTCPPort_textChanged(const QString &arg1);
	void on_leSourceTCPPass_textChanged(const QString &arg1);
	void on_leSourceTCPDesc_textChanged(const QString &arg1);
	void on_treeViewFileSystem_clicked(const QModelIndex &index);
	void on_btnFileAdd_clicked();
	void on_btnFileRemove_clicked();
	void on_btnExtract_clicked();
	void on_btnViewLog_clicked();

	void on_btnOptions_clicked();
	void on_optionsWindow_closed();
	void on_cmdLine_updated();
	void on_ccextractor_message();
	void on_ccextractor_log();
	void on_menuBar_exit_clicked();
	void on_menuBar_about_clicked();

signals:
	void cmdLineUpdated();

private:
	Ui::CCXMainWindow *ui;

	CCXOptions *optionsWindow;
	CCXAbout *aboutWindow;

	QString cmdline; //Result cmd line
	QString options; //Options set in CCXOptions Window
	QString sourceOptions; //Source options
	QString additionalOptions; //User defined additional options
	QString summaryOptions; //Options defined in Summary GroupBox

	void updateCmdLineBox();
	void updateSummary();
	void updateSourceOptions();
	QString getBinaryCmd();
	QString getLogFileName();

	QProcess *extractionProcess;
	QFile *logFile;
	QString logFileName;
};

#endif // CCXMAINWINDOW_H
