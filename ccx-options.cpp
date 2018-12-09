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


    // list with input formats
	ui->cbInputType->addItem("Auto", "");
    ui->cbInputType->addItem("Transport Stream (TS)", "-in=ts");
    ui->cbInputType->addItem("Program Stream (PS)", "-in=ps");
    ui->cbInputType->addItem("Elementary stream (ES)", "-in=es");
    ui->cbInputType->addItem("ASF (DVR-MS, etc.)", "-in=asf");
    ui->cbInputType->addItem("Microsoft WTV", "-in=wtv");
    ui->cbInputType->addItem("CCExtractor binary format", "-in=bin");
    ui->cbInputType->addItem("McPoodle Raw", "-in=raw");
	ui->cbInputType->addItem("MP4 (mov, m4v, etc.)", "-in=mp4");
    ui->cbInputType->addItem("BDAV MPEG 2 Transport Stream (M2TS)", "-in=m2ts");
    ui->cbInputType->addItem("Matroska container and WebM", "-in=mkv");
    ui->cbInputType->addItem("Material Exchange Format (MXF)", "-in=mxf");
	ui->cbInputType->addItem("wtvccdump", "-in=hex");

    // list with output formats
    ui->cbOutputType->addItem("Auto", "");
    ui->cbOutputType->addItem("srt (SubRip)", "-out=srt");
    ui->cbOutputType->addItem("SubStation Alpha", "-out=ass");
    ui->cbOutputType->addItem("WebVTT format", "-out=webvtt");
    ui->cbOutputType->addItem("WebVTT format with styling", "-out=webvtt-full");
	ui->cbOutputType->addItem("sami (Microsoft SAMI)", "-out=sami");
    ui->cbOutputType->addItem("bin (CCExtractor binary)", "-out=bin");
    ui->cbOutputType->addItem("raw (McPoodle Broadcast)", "-out=raw");
    ui->cbOutputType->addItem("dvdraw (McPoodle DVD)", "-out=dvdraw");
	ui->cbOutputType->addItem("txt (Transcript)", "-out=txt");
	ui->cbOutputType->addItem("ttxt (Timed Transcript)", "-out=ttxt");
    ui->cbOutputType->addItem("Grid 608 format", "-out=g608");
    ui->cbOutputType->addItem("SMPTE Timed Text (W3C TTML) format", "-out=smptett");
    ui->cbOutputType->addItem("spupng (XML and PNG files)", "-out=spupng");
	ui->cbOutputType->addItem("null (No output)", "-out=null");
    ui->cbOutputType->addItem("stdout (Write to stdout)", "-out=report");

    // list with split options
    ui->cbSplitType->addItem("Unrelated files", "");
    ui->cbSplitType->addItem("Parts of the same video, cut with a video tool", "-ve");

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
	options += this->getOptionsCreditsString();
	options += this->getOptionsHardsubxString();
    options += this->getOptionsDecoderString();

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
    inputOptions += ui->cbMiscStream->isChecked() ? " -s" : "";
    inputOptions += ui->cbMiscWTVMPEG2->isChecked() ? " -wtvmpeg" : "";
    inputOptions += ui->cbMiscNoscte20->isChecked() ? " --noscte20" : "";
    inputOptions += ui->cbMiscWebvttCSS->isChecked() ? " --webvtt-create-css" : "";
    inputOptions += ui->cbMiscDeblev->isChecked() ? " -deblev" : "";
    inputOptions += ui->cbMiscAnvid->isChecked() ? " -anvid" : "";

    // options for teletext
	inputOptions += ui->rbTeletextForce->isChecked() ? " -teletext" : "";

    // Levenstein distance
    if (ui->cbTeletextUseLevDist->isChecked() && !ui->rbTeletextDisable->isChecked()) {
        if (ui->cbTeletextMinLevDist->isChecked() && ui->teTeletextMinLevDist->toPlainText() != ""){
            inputOptions += " -levdistmincnt " + ui->teTeletextMinLevDist->toPlainText();
        }
        if (ui->cbTeletextMaxLevDist->isChecked() && ui->teTeletextMaxLevDist->toPlainText() != "") {
            inputOptions += " -levdistmaxcnt " + ui->teTeletextMaxLevDist->toPlainText();
        }
    } else {
        inputOptions += " -nolevdist";
    }

	inputOptions += ui->rbTeletextDisable->isChecked() ? " -noteletext" : "";

	if (ui->cbTeletextPage->isChecked()) {
		inputOptions += " -tpage " + QString::number(ui->sbTeletextPage->value());
	}

    // ES codecs
    inputOptions += ui->rbESDVB->isChecked() ? " -codec dvbsub" : "";
    inputOptions += ui->rbESIgnoreDVB->isChecked() ? " -nocodec dvbsub" : "";
    inputOptions += ui->rbTeletextCodec->isChecked() ? " -codec teletext" : "";
    inputOptions += ui->rbTeletextIgnoreCodec->isChecked() ? " -nocodec teletext" : "";

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
    debugOptions += ui->cbTSDumpDefective->isChecked() ? " -dumpdef" : "";
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
    outputOptions += ui->cbOutputSentenceCap->isChecked() ? " -c" : "";

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

    // Output (2)

    // clean data
    outputOptions += ui->cbCleanData->isChecked() ? " -cf " + this->getInfoOutputPath() : "";
    //BOM
    outputOptions += ui->cbDontAppendBOM->isChecked() ? " -nobom" : "";
    // html escapes
    outputOptions += ui->cbNoHTMLEscape->isChecked() ? " -nohtmlescape" : "";
    // split sentence
    outputOptions += ui->cbSentenceSplit->isChecked() ? " -sbs" : "";
    // XDS
    outputOptions += ui->cbSaveXDS->isChecked() ? " -xds" : "";
    // pts jumps
    if (ui->rbIgnorePTSJumps->isChecked()) {
        outputOptions += " -ignoreptsjumps";
    } else if (ui->rbFixPTSJumps->isChecked()) {
        outputOptions += " -fixptsjumps";
    }
    // SEM
    outputOptions += ui->cbCreateSem->isChecked() ? " -sem" : "";
    // languages
    if (ui->cbDVBLanguage->isChecked() && ui->leDVBLanguage->text() != "") {
        outputOptions += " -dvblang "+ui->leDVBLanguage->text();
    }
    if (ui->cbAnotherLanguage->isChecked() && ui->leAnotherLanguage->text() != "") {
        outputOptions += " -ocrlang "+ui->leAnotherLanguage->text();
    }
    // XMLTV options
    outputOptions += ui->rbXMLTVLive->isChecked() ? " -xmltv 2" : "";
    outputOptions += ui->rbXMLTVFull->isChecked() ? " -xmltv 1" : "";
    outputOptions += ui->rbXMLTVBoth->isChecked() ? " -xmltv 3" : "";
    // quantizing
    outputOptions += ui->rbQuantNo->isChecked() ? " -quant 0" : "";
    outputOptions += ui->rbQuantCCX->isChecked() ? " -quant 1" : "";
    outputOptions += ui->rbQuantReduceDistinctColor->isChecked() ? " -quant 2" : "";
    // OEM
    outputOptions += ui->rbOEMDefault->isChecked() ? " -oem 0" : "";
    outputOptions += ui->rbOEMLSTM->isChecked() ? " -oem 1" : "";
    outputOptions += ui->rbOEMBoth->isChecked() ? " -oem 2" : "";
    // DVB & OCR
    outputOptions += ui->cbDVBDontUseOCR->isChecked() ? " -nospupngocr" : "";
    // Cutom fonts
    if (ui->cbCustomFont->isChecked() && ui->leCustomFont->text() != ""){
        outputOptions += " -font "+ui->leCustomFont->text();
    }
    // Custom TTXT output file
    QString customOutTTXT[7] = {"0000000"};
    customOutTTXT[0] = ui->cbTTXTStartTime->isChecked() ? "1": "0";
    customOutTTXT[1] = ui->cbTTXTEndTime->isChecked() ? "1": "0";
    customOutTTXT[2] = ui->cbTTXTCaptionMode->isChecked() ? "1": "0";
    customOutTTXT[3] = ui->cbTTXTCaptionChannel->isChecked() ? "1": "0";
    customOutTTXT[4] = ui->cbTTXTRelativeTimestamp->isChecked() ? "1": "0";
    customOutTTXT[5] = ui->cbTTXTXDSInfo->isChecked() ? "1": "0";
    customOutTTXT[6] = ui->cbTTXTUseColors->isChecked() ? "1": "0";
    if (ui->cbCustomTTXTFile->isChecked()) {
        outputOptions += " -customtxt "+customOutTTXT[0]+
        customOutTTXT[1]+customOutTTXT[2]+
        customOutTTXT[3]+customOutTTXT[4]+
        customOutTTXT[5]+customOutTTXT[6];
    }

    //translate
    if (ui->cbTranslate->isChecked() && ui->leAPITranslate->text() != "" && ui->leTranslate->text() != "") {
        outputOptions += " -translate "+ui->leTranslate->text();
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
    // Force flush
    decoderOptions += ui->cbBufferFlush->isChecked() ? " -nospupngocr" : "";
    // KOC
    decoderOptions += ui->cbBufferKOC->isChecked() ? " -koc" : "";
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
        if (ui->hsTicker->isChecked()) {
            hardsubxOptions += " -tickertext";
        }
	}

	return hardsubxOptions;
}

void CCXOptions::on_cbTranslate_toggled(bool checked)
{
    ui->gbTranslate->setEnabled(checked);
}
void CCXOptions::on_cbCustomTTXTFile_toggled(bool checked)
{
    ui->gbCustomTTXTFileOutput->setEnabled(checked);
}

void CCXOptions::on_cbAnotherLanguage_toggled(bool checked)
{
    ui->leAnotherLanguage->setEnabled(checked);
}

void CCXOptions::on_cbCustomFont_toggled(bool checked)
{
    ui->leCustomFont->setEnabled(checked);
}

void CCXOptions::on_cbDVBLanguage_toggled(bool checked)
{
    ui->leDVBLanguage->setEnabled(checked);
}

void CCXOptions::on_cbTeletextUseLevDist_toggled(bool checked)
{
    ui->gbLevDistParams->setEnabled(checked);
}

void CCXOptions::on_cbInputType_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    if (ui->cbInputType->currentData().toString() == "-in=es" ||
        ui->cbInputType->currentData().toString() == "" ) {
            ui->cbCleanData->setEnabled(true);
            ui->rbESDVB->setEnabled(true);
            ui->rbTeletextCodec->setEnabled(true);
            ui->rbESIgnoreDVB->setEnabled(true);
            ui->rbTeletextIgnoreCodec->setEnabled(true);
    } else {
            ui->cbCleanData->setEnabled(false);
            ui->rbESDVB->setEnabled(false);
            ui->rbTeletextCodec->setEnabled(false);
            ui->rbESIgnoreDVB->setEnabled(false);
            ui->rbTeletextIgnoreCodec->setEnabled(false);
    }

    if (ui->cbInputType->currentData().toString() == "-in=ts" ||
        ui->cbInputType->currentData().toString() == "") {
            ui->gbXMLTVParams->setEnabled(true);
            ui->cbTSDumpDefective->setEnabled(true);
    } else {
            ui->gbXMLTVParams->setEnabled(false);
            ui->cbTSDumpDefective->setEnabled(false);
    }
}

void CCXOptions::on_cbHardsubx_toggled(bool checked)
{
	ui->gbSubColor->setEnabled(checked);
	ui->gbOcrMode->setEnabled(checked);
	ui->gbConfThresh->setEnabled(checked);
	ui->gbLumThresh->setEnabled(checked);
	ui->cbEnableItalicDetection->setEnabled(checked);
	ui->gbMinSubDuration->setEnabled(checked);
    ui->hsTicker->setEnabled(checked);
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
	} else {
		ui->gbOutputTranscript->setEnabled(false);
	}

	if (ui->cbOutputType->currentData().toString() == "" || //srt
        ui->cbOutputType->currentData().toString() == "-out=sami") {
            ui->cbOutputNoTypeTags->setEnabled(true);
            ui->cbOutputNoColorTags->setEnabled(true);
	} else {
		ui->cbOutputNoTypeTags->setEnabled(false);
		ui->cbOutputNoColorTags->setEnabled(false);
	}

    if (ui->cbOutputType->currentData().toString() == "-out=srt" ||
        ui->cbOutputType->currentData().toString() == "-out=sami" ||
        ui->cbOutputType->currentData().toString() == "-out=vtt" ||
        ui->cbOutputType->currentData().toString() == "") {
            ui->cbNoHTMLEscape->setEnabled(true);
    } else {
        ui->cbNoHTMLEscape->setEnabled(false);
    }

    if (ui->cbOutputType->currentData().toString() == "-out=ttxt") {
        ui->cbSaveXDS->setEnabled(true);
        ui->cbCustomTTXTFile->setEnabled(true);
    } else {
        ui->cbTTXTStartTime->setChecked(false);
        ui->cbTTXTEndTime->setChecked(false);
        ui->cbTTXTCaptionMode->setChecked(false);
        ui->cbTTXTCaptionChannel->setChecked(false);
        ui->cbTTXTRelativeTimestamp->setChecked(false);
        ui->cbTTXTXDSInfo->setChecked(false);
        ui->cbTTXTUseColors->setChecked(false);
        ui->cbSaveXDS->setEnabled(false);
        ui->gbCustomTTXTFileOutput->setEnabled(false);
        ui->cbCustomTTXTFile->setEnabled(false);
        ui->cbCustomTTXTFile->setChecked(false);
    }

    if (ui->cbOutputType->currentData().toString() == "-out=spupng") {
        ui->cbDVBDontUseOCR->setEnabled(true);
    } else {
        ui->cbDVBDontUseOCR->setEnabled(false);
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
