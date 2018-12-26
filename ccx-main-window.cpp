#include "ccx-main-window.h"
#include "ui_ccx-main-window.h"

#include <QDebug>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QDesktopServices>
#include <QDateTime>
#include <QMimeData>
#include <QDragEnterEvent>

CCXMainWindow::CCXMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::CCXMainWindow)
{
      setAcceptDrops(true);
	ui->setupUi(this);
	this->setFixedSize(this->width(), this->height());
	optionsWindow = new CCXOptions();
	aboutWindow = NULL;
	extractionProcess = NULL;

	connect(ui->btnOptions, SIGNAL(clicked()), this, SLOT(on_btnOptions_clicked()));
	connect(optionsWindow, SIGNAL(onWidgetClosed()),
			this, SLOT(on_optionsWindow_closed()));
	connect(this, SIGNAL(cmdLineUpdated(void)), this, SLOT(on_cmdLine_updated(void)));

	connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(on_menuBar_exit_clicked()));
	connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(on_menuBar_about_clicked()));

	options = "";
	additionalOptions = "";
	sourceOptions = "";
	summaryOptions = "";

	QFileSystemModel *model = new QFileSystemModel();
	model->setRootPath(QDir::rootPath());
	ui->treeViewFileSystem->setModel(model);
	ui->treeViewFileSystem->hideColumn(1);
	ui->treeViewFileSystem->hideColumn(2);
	ui->treeViewFileSystem->hideColumn(3);
	ui->treeViewFileSystem->setCurrentIndex(model->index(QDir::currentPath()));

	this->updateSummary();

	emit cmdLineUpdated();

	qDebug() << "current directory: " + QDir::currentPath();
}

CCXMainWindow::~CCXMainWindow()
{
	optionsWindow->close();
	if (extractionProcess) {
		extractionProcess->terminate();
	}
	delete ui;
	delete optionsWindow;
}

void CCXMainWindow::on_btnOptions_clicked()
{
	optionsWindow->show();
}

void CCXMainWindow::on_optionsWindow_closed()
{
	options = optionsWindow->getCommandLineOptionsString();
	qDebug() << "options updated: " << options;
	this->updateSummary();
	emit cmdLineUpdated();
}

void CCXMainWindow::updateSummary()
{
	ui->labelInputValue->setText(optionsWindow->getInfoInput());
	ui->labelOutputValue->setText(optionsWindow->getInfoOutputType());
	ui->labelOutputPathValue->setText(optionsWindow->getInfoOutputPath());
}

void CCXMainWindow::on_cmdLine_updated()
{
	cmdline = this->getBinaryCmd();
	if (sourceOptions.length()) {
		cmdline += " " + sourceOptions;
	}
	if (options.length()) {
		cmdline += " " + options;
	}
	if (summaryOptions.length()) {
		cmdline += " " + summaryOptions;
	}
	if (additionalOptions.length()) {
		cmdline += " " + additionalOptions;
	}
	ui->pteCommandLine->setPlainText(cmdline);
}

QString CCXMainWindow::getBinaryCmd()
{
#ifdef Q_OS_WIN
	return "ccextractor.exe";
#else
	return "./ccextractor";
#endif
}

void CCXMainWindow::on_leAdditionalOptions_textChanged(const QString &arg1)
{
	additionalOptions = arg1;
	qDebug() << "additional options updated: " << additionalOptions;
	emit cmdLineUpdated();
}

void CCXMainWindow::on_cbVerboseLog_toggled(bool checked)
{
	if (checked) {
		summaryOptions = "-debug";
	} else {
		summaryOptions = "";
	}
	emit cmdLineUpdated();
}

void CCXMainWindow::updateSourceOptions()
{
	int index = ui->twSource->currentIndex();
	switch (index) {
		case 0: //files
			{
				sourceOptions = "";
				for (int i = 0; i < ui->lwFiles->count(); i++) {
					sourceOptions += " " + ui->lwFiles->item(i)->text();
				}
			}
			break;
		case 1: //filesystem
			{
				sourceOptions = "";
				QModelIndexList list = ui->treeViewFileSystem->selectionModel()->selectedIndexes();
				QFileSystemModel* model = (QFileSystemModel*)ui->treeViewFileSystem->model();
				int row = -1;
				foreach (QModelIndex index, list)
				{
					if (index.row() != row && !index.column())
					{
						QFileInfo fileInfo = model->fileInfo(index);
						sourceOptions += " " + fileInfo.filePath();
						row = index.row();
					}
				}
			}
			break;
		case 2: //network
			{
				QString UDPhost = ui->leSourceUDPHost->text(),
					UDPport = ui->leSourceUDPPort->text();
				QString TCPport = ui->leSourceTCPPort->text(),
					TCPpass = ui->leSourceTCPPass->text(),
					TCPdesc = ui->leSourceTCPDesc->text();
				sourceOptions = "";
				if (UDPport.length()) {
					ui->leSourceTCPPort->setEnabled(false);
					ui->leSourceTCPPass->setEnabled(false);
					ui->leSourceTCPDesc->setEnabled(false);
					sourceOptions = " -udp ";
					sourceOptions += UDPhost.length() ? UDPhost + ":" : "";
					sourceOptions += UDPport;
				} else {
					ui->leSourceTCPPort->setEnabled(true);
					ui->leSourceTCPPass->setEnabled(true);
					ui->leSourceTCPDesc->setEnabled(true);
					if(TCPport.length()) {
						ui->leSourceUDPHost->setEnabled(false);
						ui->leSourceUDPPort->setEnabled(false);
						sourceOptions = " -tcp " + TCPport;
						sourceOptions += TCPpass.length() ? " -tcppassword " + TCPpass : "";
						sourceOptions += TCPdesc.length() ? " -tcpdesc " + TCPdesc : "";
					} else {
						ui->leSourceUDPHost->setEnabled(true);
						ui->leSourceUDPPort->setEnabled(true);
					}
				}
			}
			break;
	}
	emit cmdLineUpdated();
}

void CCXMainWindow::on_twSource_currentChanged(int index)
{
	Q_UNUSED(index);
	this->updateSourceOptions();
}

void CCXMainWindow::on_leSourceUDPHost_textChanged(const QString &arg1)
{
	Q_UNUSED(arg1);
	this->updateSourceOptions();
}

void CCXMainWindow::on_leSourceUDPPort_textChanged(const QString &arg1)
{
	Q_UNUSED(arg1);
	this->updateSourceOptions();
}

void CCXMainWindow::on_leSourceTCPPort_textChanged(const QString &arg1)
{
	Q_UNUSED(arg1);
	this->updateSourceOptions();
}

void CCXMainWindow::on_leSourceTCPPass_textChanged(const QString &arg1)
{
	Q_UNUSED(arg1);
	this->updateSourceOptions();
}

void CCXMainWindow::on_leSourceTCPDesc_textChanged(const QString &arg1)
{
	Q_UNUSED(arg1);
	this->updateSourceOptions();
}

void CCXMainWindow::on_treeViewFileSystem_clicked(const QModelIndex &index)
{
	Q_UNUSED(index);
	this->updateSourceOptions();
}

void CCXMainWindow::on_btnFileAdd_clicked()
{
	QString filePath = QFileDialog::getOpenFileName(this);
	ui->lwFiles->addItem(filePath);
	this->updateSourceOptions();
}

void CCXMainWindow::on_btnFileRemove_clicked()
{
	QList<QListWidgetItem *> items = ui->lwFiles->selectedItems();
	qDeleteAll(items);
	this->updateSourceOptions();
}

void CCXMainWindow::on_btnExtract_clicked()
{
	QString cmd = cmdline + " --gui_mode_reports";

	qDebug() << "extracting subtitles";
	qDebug() << "commandline:";
	qDebug() << cmd;

	ui->btnViewLog->setEnabled(false);

	extractionProcess = new QProcess(this);
	QCoreApplication::processEvents();

	connect(extractionProcess, SIGNAL(readyReadStandardError(void)),
			this, SLOT(on_ccextractor_message(void)));
	connect(extractionProcess, SIGNAL(readyReadStandardOutput(void)),
			this, SLOT(on_ccextractor_log(void)));

	logFileName = this->getLogFileName();
	logFile = new QFile(logFileName);
	logFile->open(QFile::WriteOnly);

	extractionProcess->start(cmd);
	extractionProcess->waitForFinished();
	qDebug() << "done";

	delete extractionProcess;
	extractionProcess = NULL;
	logFile->close();
	delete logFile;
	logFile = NULL;
}

void CCXMainWindow::on_ccextractor_message()
{
	QByteArray msg = extractionProcess->readAllStandardError();
	//qDebug() << msg;
	QString commands(msg);

	QStringList cmds = commands.split('\n');
	for (int i = 0; i < cmds.count(); i++) {
		qDebug() << "cmds[" << i << "]=" << cmds[i];
		cmds[i].replace("###", "");
		QStringList cmd = cmds[i].split('#');
		//for (int j = 0; j < cmd.count(); j++) {
		//	qDebug() << "cmd[" << j << "]=" << cmd[j];
		//}
		if (cmd[0] == "PROGRESS") {
			int progress = cmd[1].toInt();
			ui->pbExtraction->setValue(progress);
			if (progress == 100) {
				ui->btnViewLog->setEnabled(true);
			}
	   }
	}
}

void CCXMainWindow::on_ccextractor_log()
{
	QTextStream out(logFile);
	QString logLine;
	while (extractionProcess->canReadLine()) {
		logLine = extractionProcess->readLine();
	   out << logLine;
	}
}

QString CCXMainWindow::getLogFileName()
{
	QString name = QDateTime::currentDateTime().toString("ccx_yyyy.MM.dd_hh-mm-ss");
	name += ".txt";
	return name;
}

void CCXMainWindow::on_btnViewLog_clicked()
{
	QDesktopServices::openUrl(QUrl(logFileName));
}

void CCXMainWindow::on_menuBar_exit_clicked()
{
	this->close();
}

void CCXMainWindow::on_menuBar_about_clicked()
{
	if (!aboutWindow) {
	   aboutWindow = new CCXAbout();
	}
	aboutWindow->show();
}
void CCXMainWindow::dragEnterEvent(QDragEnterEvent *e)
{
       if (e->mimeData()->hasUrls()) {
           e->acceptProposedAction();
       }
}
void CCXMainWindow::dropEvent(QDropEvent *e)
{
        foreach (const QUrl &url, e->mimeData()->urls()) {
        QString droppedFileName = url.toLocalFile();
        ui->lwFiles->addItem(droppedFileName);
        this->updateSourceOptions();
    }
}
