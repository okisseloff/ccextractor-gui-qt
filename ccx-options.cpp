#include "ccx-options.h"
#include "ui_ccx-options.h"
#include <QDebug>
#include <QFileDialog>

CCXOptions::CCXOptions(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CCXOptions)
{
	ui->setupUi(this);
	this->setFixedSize(this->width(), this->height());

	ui->cbInputType->addItem("Auto", "");
	ui->cbInputType->addItem("MP4 (mov, m4v, etc.)", "-in=mp4");
	ui->cbInputType->addItem("Program Stream (PS)", "-in=ps");
	ui->cbInputType->addItem("Transport Stream (TS)", "-in=ts");
	ui->cbInputType->addItem("BDAV MPEG 2 Transport Stream (M2TS)", "-in=m2ts");
	ui->cbInputType->addItem("Elementary stream (ES)", "-in=es");
	ui->cbInputType->addItem("Microsoft WTV", "-in=wtv");
	ui->cbInputType->addItem("ASF (DVR-MS, etc.)", "-in=asf");
	ui->cbInputType->addItem("McPoodle Raw", "-in=raw");
	ui->cbInputType->addItem("wtvccdump", "-in=hex");
	ui->cbInputType->addItem("CCExtractor binary format", "-in=bin");

	ui->cbSplitType->addItem("Unrelated files", "");
	ui->cbSplitType->addItem("Parts of the same video, cut with a video tool", "-ve");

	ui->cbOutputType->addItem("srt (SubRip)", "");
	ui->cbOutputType->addItem("sami (Microsoft SAMI)", "-out=sami");
	ui->cbOutputType->addItem("txt (Transcript)", "-out=txt");
	ui->cbOutputType->addItem("ttxt (Timed Transcript)", "-out=ttxt");
	ui->cbOutputType->addItem("smptett (SMPTE Timed Text (W3C TTML))", "-out=smptett");
	ui->cbOutputType->addItem("null (No output)", "-out=null");
	ui->cbOutputType->addItem("spupng (XML and PNG files)", "-out=spupng");
	ui->cbOutputType->addItem("bin (CCExtractor binary)", "-out=bin");
	ui->cbOutputType->addItem("raw (McPoodle Broadcast)", "-out=raw");
	ui->cbOutputType->addItem("dvdraw (McPoodle DVD)", "-out=dvdraw");
	ui->cbOutputType->addItem("ass (SubStation Alpha)", "-out=ass");
	ui->cbOutputType->addItem("ssa (SubStation Alpha)", "-out=ssa");
	ui->cbOutputType->addItem("webvtt (WebVTT format)", "-out=webvtt");
	ui->cbOutputType->addItem("report (stdout caption info)", "-out=report");
	ui->cbOutputType->addItem("stdout (Write to stdout)", "-stdout");

	this->setWindowTitle("CCExtractor options");
}

CCXOptions::~CCXOptions()
{
	delete ui;
}

void CCXOptions::closeEvent(QCloseEvent *e) {
  QWidget::closeEvent(e);
  emit onWidgetClosed();
}

QString CCXOptions::getInfoInput()
{
	QString infoInput;
	infoInput += ui->cbInputType->currentText();
	return infoInput;
}

QString CCXOptions::getInfoOutputType()
{
	QString infoOutputType;
	infoOutputType += ui->cbOutputType->currentText();
	return infoOutputType;
}

QString CCXOptions::getInfoOutputPath()
{
	QString infoOutputPath;
	if (ui->cbOutputPath->isChecked()) {
		infoOutputPath += ui->leOutputPath->text();
	} else {
		infoOutputPath += "Default";
	}
	return infoOutputPath;
}

QString CCXOptions::getCommandLineOptionsString()
{
	QString options;

	options += this->getOptionsInputString();
	options += this->getOptionsDebugString();
	options += this->getOptionsOutputString();
	options += this->getOptionsDecoderString();
	options += this->getOptionsCreditsString();
	options += this->getOptionsHardsubxString();

	return options;
}

QString CCXOptions::getOptionsInputString()
{
	QString inputOptions;

	if (ui->cbInputType->currentData().toString().length()) {
		inputOptions += " " + ui->cbInputType->currentData().toString();
	}
	if (ui->cbSplitType->currentData().toString().length()) {
		inputOptions += " " + ui->cbSplitType->currentData().toString();
	}

	if (ui->cbLiveStream->isChecked()) {
		inputOptions += " -s";
		if (ui->sbLiveStreamWait->value()) {
			inputOptions += " " + QString::number(ui->sbLiveStreamWait->value());
		}
	}
	if (ui->cbStartAt->isChecked()) {
		inputOptions += " -startat " + ui->teStartAt->time().toString();
	}
	if (ui->cbEndAt->isChecked()) {
		inputOptions += " -endat " + ui->teEndAt->time().toString();
	}
	if (ui->cbScreenfuls->isChecked() && ui->sbScreenfulsCount->value()) {
		inputOptions += " -scr " + QString::number(ui->sbScreenfulsCount->value());
	}
	if (ui->rbESType->isChecked()) {
		inputOptions += " -datastreamtype " + QString::number(ui->sbESType->value(), 16);
	}
	if (ui->rbESPID->isChecked()) {
		inputOptions += " -datapid " + QString::number(ui->sbESPID->value(), 16);
	}
	if (ui->cbTreatMPEGTypeAs->isChecked()) {
		inputOptions += " -streamtype " + QString::number(ui->sbTreatMPEGTypeAs->value());
	}
	if (ui->cbMultiprogram->isChecked()) {
		if (ui->rbMultiprogramAuto->isChecked()) {
			inputOptions += " -autoprogram";
		}
		if (ui->rbMultiprogramNumber->isChecked()) {
			inputOptions += " -pn " + QString::number(ui->sbMultiprogramNumber->value());
		}
	}

	//Input (2)
	inputOptions += ui->rbClockGOP->isChecked() ? " -gt" : "";
	inputOptions += ui->rbClockPTS->isChecked() ? " -nogt" : "";

	inputOptions += ui->rbMythTVForce->isChecked() ? " -myth" : "";
	inputOptions += ui->rbMythTVDisable->isChecked() ? " -nomyth" : "";

	inputOptions += ui->cbMisc90090->isChecked() ? " -90090" : "";
	inputOptions += ui->cbMiscFixPadding->isChecked() ? " -fp" : "";
	inputOptions += ui->cbMiscPicOrder->isChecked() ? " -poc" : "";
	inputOptions += ui->cbMiscWTV->isChecked() ? " -wtvconvertfix" : "";
	inputOptions += ui->cbMiscHaup->isChecked() ? " -haup" : "";
	inputOptions += ui->cbMiscMP4ForceVideo->isChecked() ? " -mp4vidtrack" : "";
	inputOptions += ui->cbMiscNoTimeRef->isChecked() ? " -noautotimeref" : "";

	inputOptions += ui->rbTeletextForce->isChecked() ? " -teletext" : "";
	inputOptions += ui->rbTeletextDisable->isChecked() ? " -noteletext" : "";

	inputOptions += ui->rbCodecDVB->isChecked() ? " -codec dvbsub " : "";
	inputOptions += ui->rbNocodecDVB->isChecked() ? " -nocodec dvbsub " : "";
	inputOptions += ui->rbCodecTeletext->isChecked() ? " -codec teletext " : "";
	inputOptions += ui->rbNocodecTeletext->isChecked() ? " -nocodec teletext " : "";

	inputOptions += ui->cbWTVMPEG->isChecked() ? " -wtvmpeg2" : "";
	inputOptions += ui->cbNoSCTE20->isChecked() ? " --noscte20" : "";

	if (ui->cbTeletextPage->isChecked()) {
		inputOptions += " -tpage " + QString::number(ui->sbTeletextPage->value());
	}

	return inputOptions;
}

QString CCXOptions::getOptionsDebugString()
{
	QString debugOptions;

	debugOptions += ui->cbDebug608->isChecked() ? " -608" : "";
	debugOptions += ui->cbDebug708->isChecked() ? " -708" : "";
	debugOptions += ui->cbDebugPTS->isChecked() ? " -goppts" : "";
	debugOptions += ui->cbDebugXDS->isChecked() ? " -xdsdebug" : "";
	debugOptions += ui->cbDebugES->isChecked() ? " -vides" : "";
	debugOptions += ui->cbDebugRaw->isChecked() ? " -cbraw" : "";
	debugOptions += ui->cbDebugNoSync->isChecked() ? " -nosync" : "";
	debugOptions += ui->cbDebugFullBin->isChecked() ? " -fullbin" : "";
	debugOptions += ui->cbDebugParse->isChecked() ? " -parsedebug" : "";
	debugOptions += ui->cbDebugPAT->isChecked() ? " -parsePAT" : "";
	debugOptions += ui->cbDebugPMT->isChecked() ? " -parsePMT" : "";
	debugOptions += ui->cbDebugInvestigate->isChecked() ? " -investigate_packets" : "";
	debugOptions += ui->cbDebugTeletext->isChecked() ? " -tverbose" : "";
	debugOptions += ui->cbDebugDumpdef->isChecked() ? "-dumpdef" : "";

	if (ui->cbDebugESFile->isChecked()) {
		debugOptions += " -cf " + ui->leDebugESFile->text();
	}

	return debugOptions;
}

QString CCXOptions::getOptionsOutputString()
{
	QString outputOptions;

	outputOptions += " " + ui->cbOutputType->currentData().toString();
	if (ui->cbOutputPath->isChecked()) {
		outputOptions += " -o " + ui->leOutputPath->text();
	}
	if (ui->cbSubDelay->isChecked()) {
		outputOptions += " -delay " + QString::number(ui->sbSubDelay->value());
	}
	outputOptions += ui->rbEncodingLatin1->isChecked() ? " -latin1" : "";
	outputOptions += ui->rbEncodingUnicode->isChecked() ? " -unicode" : "";
	outputOptions += ui->rbOutputNewLineLF->isChecked() ? " -lf" : "";
	outputOptions += ui->cbOutputSentenceCap->isChecked() ? " -sc" : "";

	if (ui->cbOutputCapFile->isChecked()) {
		outputOptions += " -caf " + ui->leOutputCapFile->text();
	}

	outputOptions += ui->cbOutputTrim->isChecked() ? " -trim" : "";
	outputOptions += ui->cbOutputAutodash->isChecked() ? " -autodash" : "";
	outputOptions += ui->cbOutputNoTypeTags->isChecked() ? " -nots" : "";
	outputOptions += ui->cbOutputNoColorTags->isChecked() ? " -nofc" : "";

	if (ui->cbOutputDefaultColor->isChecked()) {
		//TODO check if value is valid #RRGGBB
		outputOptions += " -dc " + ui->leOutputDefaultColor->text();
	}

	if (ui->cbOutputTranscriptUnixRef->isChecked()) {
		outputOptions += " -unixts " + ui->leOutputTranscriptUnixRef->text();
	}

	outputOptions += ui->cbOutputTranscriptUCLA->isChecked() ? " -UCLA" : "";
	outputOptions += ui->rbOutputTranscriptTimeDate->isChecked() ? " -datets" : "";
	outputOptions += ui->rbOutputTranscriptTimeSSMS->isChecked() ? " -sects" : "";

	//Output (2)

	outputOptions += ui->rbAppendBOM->isChecked() ? " -bom" : "";
	outputOptions += ui->rbNoBOM->isChecked() ? " -nobom" : "";
	outputOptions += ui->cbNoHTMLEscape->isChecked() ? " --nohtmlescape" : "";
	outputOptions += ui->cbXMLTV->isChecked() ? " -xmltv" : "";
	outputOptions += ui->cbSEM->isChecked() ? " -sem" : "";

	if (ui->cbOutputInterval->isChecked()){
		outputOptions += " -outinterval " + ui->leOutputInterval->text();
	}

	if (ui->gbOutputTranscriptCustom->isEnabled()){
		outputOptions += " -customtxt ";
		outputOptions += ui->cbTranscriptStart->isChecked() ? "1" : "0";
		outputOptions += ui->cbTranscriptEnd->isChecked() ? "1" : "0";
		outputOptions += ui->cbTranscriptMode->isChecked() ? "1" : "0";
		outputOptions += ui->cbTranscriptChannel->isChecked() ? "1" : "0";
		outputOptions += ui->cbTranscriptRelative->isChecked() ? "1" : "0";
		outputOptions += ui->cbTranscriptXDS->isChecked() ? "1" : "0";
		outputOptions += ui->cbTranscriptColors->isChecked() ? "1" : "0";
	}

	return outputOptions;
}

QString CCXOptions::getOptionsDecoderString()
{
	QString decoderOptions;

	decoderOptions += ui->rbDecoderBufferForce->isChecked() ? " -bi": "";
	decoderOptions += ui->rbDecoderBufferDisable->isChecked() ? " -nobi": "";
	if (ui->cbDecoderBufferSize->isChecked()) {
		decoderOptions += " -bs " + ui->leDecoderBufferSize->text();
	}
	decoderOptions += ui->cbKOC->isChecked() ? " -koc" : "";
	decoderOptions += ui->cbForceFlush->isChecked() ? " -ff": "";

	decoderOptions += ui->cbDecoderRollUpDirect->isChecked() ? " -dru" : "";
	decoderOptions += ui->cbDecoderRollUpLineOnce->isChecked() ? " -noru" : "";

	if (ui->cbDecoderRollUpLimit->isChecked()) {
		decoderOptions += ui->rbDecoderRollUpLimit1->isChecked() ? " -ru1" : "";
		decoderOptions += ui->rbDecoderRollUpLimit2->isChecked() ? " -ru2" : "";
		decoderOptions += ui->rbDecoderRollUpLimit3->isChecked() ? " -ru3" : "";
	}

	bool field1 = ui->cbDecoderField1->isChecked(),
		field2 = ui->cbDecoderField2->isChecked();

	if (field1 && field2) {
		decoderOptions += " -12";
	} else if (field2) {
		decoderOptions += " -2";
	} else {
		//by default we handle only field 1
	}

	decoderOptions += ui->cbDecoderChannel2->isChecked() ? " -cc2" : "";

	decoderOptions += ui->cbCEA708EnableServices->isChecked() ? " -svc " + ui->leCEA708Services->text() : "";

	return decoderOptions;
}

QString CCXOptions::getOptionsCreditsString()
{
	QString creditsOptions;
	if (ui->cbEnableStartCredits->isChecked()) {
		creditsOptions += " --startcreditstext " +
				ui->pteCreditsStart->toPlainText();
		if (ui->cbCreditsStartAfter->isChecked()) {
			creditsOptions += " --startcreditsnotbefore " +
					ui->teCreditsAfter->time().toString("mm:ss");
		}
		if (ui->cbCreditsStartBefore->isChecked()) {
			creditsOptions += " --startcreditsnotafter " +
					ui->teCreditsBefore->time().toString("mm:ss");
		}
		if (ui->cbCreditsStartDisplay->isChecked()) {
			creditsOptions += " --startcreditsforatleast " +
					QString::number(ui->sbCreditsStartAtLeast->value());
			creditsOptions += " --startcreditsforatmost " +
					QString::number(ui->sbCreditsStartAtMost->value());
		}
	}
	if (ui->cbEnableEndCredits->isChecked()) {
		creditsOptions += " --endcreditstext " +
				ui->pteCreditsEnd->toPlainText();
		if (ui->cbCreditsEndDisplay->isChecked()) {
			creditsOptions += " --endcreditsforatleast " +
					QString::number(ui->sbCreditsEndAtLeast->value());
			creditsOptions += " --endcreditsforatmost " +
					QString::number(ui->sbCreditsEndAtMost->value());
		}
	}

	return creditsOptions;
}

QString CCXOptions::getOptionsHardsubxString()
{
	QString hardsubxOptions;

	if (ui->cbHardsubx->isChecked()) {
		hardsubxOptions += " -hardsubx";

		//Subtitle color
		if (ui->rbSubColorWhite->isChecked()) {
			hardsubxOptions += " -subcolor white";
		} else if (ui->rbSubColorYellow->isChecked()) {
			hardsubxOptions += " -subcolor yellow";
		} else if (ui->rbSubColorGreen->isChecked()) {
			hardsubxOptions += " -subcolor green";
		} else if (ui->rbSubColorCyan->isChecked()) {
			hardsubxOptions += " -subcolor cyan";
		} else if (ui->rbSubColorBlue->isChecked()) {
			hardsubxOptions += " -subcolor blue";
		} else if (ui->rbSubColorMagenta->isChecked()) {
			hardsubxOptions += " -subcolor magenta";
		} else if (ui->rbSubColorRed->isChecked()) {
			hardsubxOptions += " -subcolor red";
		} else if (ui->rbSubColorCustom->isChecked()) {
			hardsubxOptions += " -subcolor " + ui->leSubColorCustom->text();
		}

		//OCR Mode
		if (ui->rbOcrModeFrame->isChecked()) {
			hardsubxOptions += " -ocr_mode frame";
		} else if (ui->rbOcrModeWord->isChecked()) {
			hardsubxOptions += " -ocr_mode word";
		} else if (ui->rbOcrModeLetter->isChecked()) {
			hardsubxOptions += " -ocr_mode letter";
		}

		//Detect Italics
		if (ui->cbEnableItalicDetection->isChecked()) {
			hardsubxOptions += " -detect_italics";
		}

		//Minimum Subtitle Duration
		hardsubxOptions += " -min_sub_duration " + ui->leMinSubDuration->text();

		//Confidence Threshold
		if (ui->hsConfThresh->value() > 0.0 && ui->hsConfThresh->value() <= 100.0) {
			hardsubxOptions += " -conf_thresh " + QString::number(ui->hsConfThresh->value());
		}

		//Luminance Threshold
		if (ui->hsLumThresh->value() > 0.0 && ui->hsLumThresh->value() <= 100.0) {
			if (ui->rbSubColorWhite->isChecked()) {
				hardsubxOptions += " -lum_thresh " + QString::number(ui->hsLumThresh->value());
			}
		}
	}

	return hardsubxOptions;
}

void CCXOptions::on_cbHardsubx_toggled(bool checked)
{
	ui->gbSubColor->setEnabled(checked);
	ui->gbOcrMode->setEnabled(checked);
	ui->gbConfThresh->setEnabled(checked);
	ui->gbLumThresh->setEnabled(checked);
	ui->cbEnableItalicDetection->setEnabled(checked);
	ui->gbMinSubDuration->setEnabled(checked);
}

void CCXOptions::on_hsLumThresh_valueChanged(int value)
{
	ui->labelLumThresh->setText(QString::number(value));
}

void CCXOptions::on_hsConfThresh_valueChanged(int value)
{
	ui->labelConfThresh->setText(QString::number(value));
}

void CCXOptions::on_cbLiveStream_toggled(bool checked)
{
	ui->sbLiveStreamWait->setEnabled(checked);
}

void CCXOptions::on_cbScreenfuls_toggled(bool checked)
{
	ui->sbScreenfulsCount->setEnabled(checked);
}

void CCXOptions::on_cbStartAt_toggled(bool checked)
{
	ui->teStartAt->setEnabled(checked);
}

void CCXOptions::on_cbEndAt_toggled(bool checked)
{
	ui->teEndAt->setEnabled(checked);
}

void CCXOptions::on_rbESType_toggled(bool checked)
{
	ui->sbESType->setEnabled(checked);
}

void CCXOptions::on_rbESPID_toggled(bool checked)
{
	ui->sbESPID->setEnabled(checked);
}

void CCXOptions::on_cbTreatMPEGTypeAs_toggled(bool checked)
{
	ui->sbTreatMPEGTypeAs->setEnabled(checked);
}

void CCXOptions::on_cbSubDelay_toggled(bool checked)
{
	ui->sbSubDelay->setEnabled(checked);
}

void CCXOptions::on_cbOutputPath_toggled(bool checked)
{
	ui->leOutputPath->setEnabled(checked);
	ui->btnOutputPath->setEnabled(checked);
}

void CCXOptions::on_cbMultiprogram_toggled(bool checked)
{
	ui->rbMultiprogramAuto->setEnabled(checked);
	ui->rbMultiprogramNumber->setEnabled(checked);
	ui->sbMultiprogramNumber->setEnabled(checked && ui->rbMultiprogramNumber->isChecked());
}

void CCXOptions::on_rbMultiprogramNumber_toggled(bool checked)
{
	ui->sbMultiprogramNumber->setEnabled(checked);
}

void CCXOptions::on_btnOutputPath_clicked()
{
	QString outputPath = QFileDialog::getExistingDirectory(this);
	qDebug() << outputPath;
	ui->leOutputPath->setText(outputPath);
}

void CCXOptions::on_cbTeletextPage_toggled(bool checked)
{
	ui->sbTeletextPage->setEnabled(checked);
}

void CCXOptions::on_cbEnableStartCredits_toggled(bool checked)
{
	ui->pteCreditsStart->setEnabled(checked);

	ui->cbCreditsStartAfter->setEnabled(checked);
	ui->teCreditsAfter->setEnabled(checked && ui->cbCreditsStartAfter->isChecked());

	ui->cbCreditsStartBefore->setEnabled(checked);
	ui->teCreditsBefore->setEnabled(checked && ui->cbCreditsStartBefore->isChecked());

	ui->cbCreditsStartDisplay->setEnabled(checked);
	ui->sbCreditsStartAtLeast->setEnabled(checked && ui->cbCreditsStartDisplay->isChecked());
	ui->sbCreditsStartAtMost->setEnabled(checked && ui->cbCreditsStartDisplay->isChecked());
	ui->labelCreditsStart1->setEnabled(checked);
	ui->labelCreditsStart2->setEnabled(checked);
}

void CCXOptions::on_cbEnableEndCredits_toggled(bool checked)
{
	ui->pteCreditsEnd->setEnabled(checked);

	ui->cbCreditsEndDisplay->setEnabled(checked);
	ui->sbCreditsEndAtLeast->setEnabled(checked && ui->cbCreditsEndDisplay->isChecked());
	ui->sbCreditsEndAtMost->setEnabled(checked && ui->cbCreditsEndDisplay->isChecked());

	ui->labelCreditsEnd1->setEnabled(checked);
	ui->labelCreditsEnd2->setEnabled(checked);
}

void CCXOptions::on_cbCreditsStartAfter_toggled(bool checked)
{
	ui->teCreditsAfter->setEnabled(checked);
}

void CCXOptions::on_cbCreditsStartBefore_toggled(bool checked)
{
	ui->teCreditsBefore->setEnabled(checked);
}

void CCXOptions::on_cbCreditsStartDisplay_toggled(bool checked)
{
	ui->sbCreditsStartAtLeast->setEnabled(checked);
	ui->sbCreditsStartAtMost->setEnabled(checked);
}

void CCXOptions::on_cbCreditsEndDisplay_toggled(bool checked)
{
	ui->sbCreditsEndAtLeast->setEnabled(checked);
	ui->sbCreditsEndAtMost->setEnabled(checked);
}

void CCXOptions::on_btnOutputCapFile_clicked()
{
	QString capFile = QFileDialog::getOpenFileName(this);
	ui->leOutputCapFile->setText(capFile);
}

void CCXOptions::on_cbOutputCapFile_toggled(bool checked)
{
	ui->btnOutputCapFile->setEnabled(checked);
	ui->leOutputCapFile->setEnabled(checked);
}

void CCXOptions::on_cbOutputDefaultColor_toggled(bool checked)
{
	ui->leOutputDefaultColor->setEnabled(checked);
}

void CCXOptions::on_cbOutputType_currentIndexChanged(int index)
{
	Q_UNUSED(index);
	if (ui->cbOutputType->currentData().toString() == "-out=txt" ||
			ui->cbOutputType->currentData().toString() == "-out=ttxt") {
		ui->gbOutputTranscript->setEnabled(true);
		ui->gbOutputTranscript2->setEnabled(true);
	} else {
		ui->gbOutputTranscript->setEnabled(false);
		ui->gbOutputTranscript2->setEnabled(false);
	}

	if (ui->cbOutputType->currentData().toString() == "" || //srt
			ui->cbOutputType->currentData().toString() == "-out=sami") {
		ui->cbOutputNoTypeTags->setEnabled(true);
		ui->cbOutputNoColorTags->setEnabled(true);
	} else {
		ui->cbOutputNoTypeTags->setEnabled(false);
		ui->cbOutputNoColorTags->setEnabled(false);
	}

	ui->cbOutputAutodash->setEnabled(ui->cbOutputType->currentData().toString() == "" && //srt
									 ui->cbOutputTrim->isChecked());
}

void CCXOptions::on_cbOutputTrim_toggled(bool checked)
{
	bool srtOrSami = ui->cbOutputType->currentData().toString() == "" ||
			ui->cbOutputType->currentData().toString() == "-out=sami";
	ui->cbOutputAutodash->setEnabled(checked && srtOrSami);
}

void CCXOptions::on_rbDecoderBufferDisable_toggled(bool checked)
{
	ui->cbDecoderBufferSize->setEnabled(!checked);
	ui->leDecoderBufferSize->setEnabled(!checked && ui->cbDecoderBufferSize->isChecked());
}

void CCXOptions::on_cbDecoderBufferSize_toggled(bool checked)
{
	ui->leDecoderBufferSize->setEnabled(checked);
}

void CCXOptions::on_btnDebugESFile_clicked()
{
	QString esFile = QFileDialog::getOpenFileName(this);
	ui->leDebugESFile->setText(esFile);
}

void CCXOptions::on_cbDebugESFile_toggled(bool checked)
{
	ui->btnDebugESFile->setEnabled(checked);
	ui->leDebugESFile->setEnabled(checked);
}

void CCXOptions::on_cbDecoderRollUpLimit_toggled(bool checked)
{
	ui->rbDecoderRollUpLimit1->setEnabled(checked);
	ui->rbDecoderRollUpLimit2->setEnabled(checked);
	ui->rbDecoderRollUpLimit3->setEnabled(checked);
}

void CCXOptions::on_cbCEA708EnableServices_toggled(bool checked)
{
	ui->leCEA708Services->setEnabled(checked);
}

void CCXOptions::on_cbOutputInterval_toggled(bool checked)
{
	ui->leOutputInterval->setEnabled(checked);
}

void CCXOptions::on_cbOutputTranscriptCustom_toggled(bool checked)
{
	ui->gbOutputTranscriptCustom->setEnabled(!checked);
}
