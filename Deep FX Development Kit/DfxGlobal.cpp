/*
* The Mailo Today Solutions Software License
*
* Copyright 2020 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#include <QThread>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox >
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QFormLayout>

#include "DfxGlobal.h"
#include "DfxCoreApplication.h"

namespace DfxGlobal {

	namespace console {

		void println(DfxString str) {
			DfxConsoleBuffer::getInstance()->appendBuffer(str + std::string("\n"));
		}

		void println(std::string str) {
			println(DfxString(str.c_str()));
		}

		void println(const char *str) {
			println(DfxString(str));
		}

		void python_println(const char *str) {
			println(DfxString(str));
		}

		void print(DfxString str) {
			DfxConsoleBuffer::getInstance()->appendBuffer(str);
		}

		void print(std::string str) {
			print(DfxString(str.c_str()));
		}

		void print(const char *str) {
			print(DfxString(str));
		}

		void python_print(const char *str) {
			print(DfxString(str));
		}
	}

	namespace info {
		void message(DfxString str) {

			if (QApplication::instance()->thread() == QThread::currentThread()) {
				DfxCoreApplication::getInstance()->statusMessage(str.c_str());
			}
			else {

				QMetaObject::invokeMethod(DfxCoreApplication::getInstance(), "statusMessage",
					Qt::AutoConnection, Q_ARG(QString, QString(str.c_str())));
			}
		}

		void message(std::string str) {
			message(DfxString(str.c_str()));
		}

		void message(const char *str) {
			message(DfxString(str));
		}

		int messageBox(std::string text, std::string info_text, int width, std::string cltext) {
			return messageBox(text.c_str(), info_text.c_str(), width, text.c_str());
		}

		int messageBox(const char *text, const char *info_text, int width, const char *cltext) {

			std::stringstream ss;
			ss << "QLabel{min-width: " << width << "px;}";

			bool yesNo = false;
			bool okCancel = false;

			if (QString(cltext).compare(QString("Yes_No" ), Qt::CaseInsensitive) == 0) {
				yesNo = true;
			}
			else if (QString(cltext).compare(QString("Ok_Cancel"), Qt::CaseInsensitive) == 0) {
				okCancel = true;
			}

			QMessageBox msgBox;
			msgBox.setText(text);
			msgBox.setInformativeText(info_text);
			msgBox.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			msgBox.setStyleSheet(ss.str().c_str());

			if ( yesNo ) {
				msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
				msgBox.setDefaultButton(QMessageBox::Yes);
			}
			else if (okCancel) {
				msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
				msgBox.setDefaultButton(QMessageBox::Ok);
			}
					
			int ret = msgBox.exec();

			switch (ret) {
			case QMessageBox::Ok:
				return 1;
				break;
			case QMessageBox::Yes:
				return 1;
				break;
			default:
				return 0;
				break;
			}
		}


		bool python_messageBox(const char *text, const char *info_text, int width, const char *cltext) {
			int rc = messageBox(text, info_text, width, cltext);

			if (rc == 0) {
				return false;
			}
			else {
				return true;
			}
		}
	}

	namespace input {

		float getFloatValue(const char *title, const char *label, float default_value) {
			QDialog dialog;

			QDialogButtonBox *buttonBox = new QDialogButtonBox(&dialog);
			QPushButton  *ok = new QPushButton("Ok");
			QPushButton  *cancel = new QPushButton("Cancel");

			QFormLayout *flayout = new QFormLayout();
			QDoubleSpinBox *value = new QDoubleSpinBox();
			value->setValue(default_value);

			buttonBox->addButton(ok, QDialogButtonBox::AcceptRole);
			buttonBox->addButton(cancel, QDialogButtonBox::RejectRole);

			flayout->addRow(QString(QString(label) + ":"), value);

			flayout->addWidget(buttonBox);

			dialog.setWindowTitle(title);
			dialog.setMinimumWidth(200);

			dialog.setLayout(flayout);

			QObject::connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);
			QObject::connect(cancel, &QPushButton::clicked, &dialog, &QDialog::reject);

			dialog.exec();

			return (float)value->value();
		}

		const char *getStringValue(const char *title, const char *label, const char *default_value) {
			QDialog dialog;

			QDialogButtonBox *buttonBox = new QDialogButtonBox(&dialog);
			QPushButton  *ok = new QPushButton("Ok");
			QPushButton  *cancel = new QPushButton("Cancel");

			QFormLayout *flayout = new QFormLayout();
			QLineEdit   *value   = new QLineEdit();
			value->setText(default_value);

			buttonBox->addButton(ok, QDialogButtonBox::AcceptRole);
			buttonBox->addButton(cancel, QDialogButtonBox::RejectRole);

			flayout->addRow(QString(QString(label) + ":"), value);

			flayout->addWidget(buttonBox);

			dialog.setWindowTitle(title);
			dialog.setMinimumWidth(200);

			dialog.setLayout(flayout);

			QObject::connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);
			QObject::connect(cancel, &QPushButton::clicked, &dialog, &QDialog::reject);

			dialog.exec();

			return value->text().toStdString().c_str();
		}
	}


	namespace command {

		void exec(DfxString cmd) {

			DfxCmdArguments args;
			QString qstring(cmd.c_str());
			QStringList strlst = qstring.split(QChar(' '));

			for (size_t inx = 0; inx < strlst.size(); inx++) {
				args.push_back(DfxString(strlst.at(inx).toUtf8().constData()));
			}

			DfxCommandCreatePtr cmdCreatePtr = DfxCommandDatabase::getInstance()->findCommand(args[0]);

			if (cmdCreatePtr == nullptr) {
				std::string msg = std::string("Command Error: failed to execute '") + std::string(args[0].c_str()) + std::string("', command not found.");
				DfxGlobal::console::println(msg);
				return;
			}

			DfxAbstractCommand *cmd_instance = (DfxAbstractCommand*)cmdCreatePtr();

			if (args.size() > 1) {
				cmd_instance->execute(args);
			}
			else {
				cmd_instance->execute();
			}

			delete cmd_instance;
		}

		void exec(std::string cmd) {
			exec(DfxString(cmd.c_str()));
		}

		void exec(const char *cmd) {
			exec(DfxString(cmd));
		}

		void python_exec(const char *cmd) {
			exec(DfxString(cmd));
		}
	}
}