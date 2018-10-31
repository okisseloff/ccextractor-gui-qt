#ifndef CCXOPTIONS_H
#define CCXOPTIONS_H

#include <QWidget>

namespace Ui {
class CCXOptions;
}

class CCXOptions : public QWidget
{
	Q_OBJECT

public:
	explicit CCXOptions(QWidget *parent = 0);
	~CCXOptions();
	QString getCommandLineOptionsString();
	QString getInfoInput();
	QString getInfoOutputType();
	QString getInfoOutputPath();

protected:
	void closeEvent(QCloseEvent *);
signals:
	void onWidgetClosed();
private slots:
	void on_cbLiveStream_toggled(bool checked);
	void on_cbScreenfuls_toggled(bool checked);
	void on_cbStartAt_toggled(bool checked);
	void on_cbEndAt_toggled(bool checked);
	void on_rbESType_toggled(bool checked);
	void on_rbESPID_toggled(bool checked);
	void on_cbTreatMPEGTypeAs_toggled(bool checked);
	void on_cbSubDelay_toggled(bool checked);
	void on_cbOutputPath_toggled(bool checked);
	void on_cbMultiprogram_toggled(bool checked);
	void on_rbMultiprogramNumber_toggled(bool checked);
	void on_btnOutputPath_clicked();
	void on_cbTeletextPage_toggled(bool checked);
	void on_cbEnableStartCredits_toggled(bool checked);
	void on_cbEnableEndCredits_toggled(bool checked);
	void on_cbCreditsStartAfter_toggled(bool checked);
	void on_cbCreditsStartBefore_toggled(bool checked);
	void on_cbCreditsStartDisplay_toggled(bool checked);
	void on_cbCreditsEndDisplay_toggled(bool checked);
	void on_btnOutputCapFile_clicked();
	void on_cbOutputCapFile_toggled(bool checked);
	void on_cbOutputDefaultColor_toggled(bool checked);
	void on_cbOutputType_currentIndexChanged(int index);
	void on_cbOutputTrim_toggled(bool checked);
	void on_cbOutputInterval_toggled(bool checked);
	void on_cbOutputTranscriptCustom_toggled(bool checked);
	void on_rbDecoderBufferDisable_toggled(bool checked);
	void on_cbDecoderBufferSize_toggled(bool checked);
	void on_btnDebugESFile_clicked();
	void on_cbDebugESFile_toggled(bool checked);
	void on_cbDecoderRollUpLimit_toggled(bool checked);
	void on_cbCEA708EnableServices_toggled(bool checked);
	void on_cbHardsubx_toggled(bool checked);
	void on_hsLumThresh_valueChanged(int value);
	void on_hsConfThresh_valueChanged(int value);

private:
	QString getOptionsInputString();
	QString getOptionsDebugString();
	QString getOptionsOutputString();
	QString getOptionsCreditsString();
	QString getOptionsDecoderString();
	QString getOptionsHardsubxString();

private:
	Ui::CCXOptions *ui;
};

#endif // CCXOPTIONS_H
