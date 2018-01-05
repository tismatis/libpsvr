#include "DataViewer.h"
#include "psvr.h"

//---------------------------------------------
//Sensor Thread
//---------------------------------------------
SensorThread::SensorThread(struct psvr_context *ctx) {
	this->ctx = ctx;
	this->running = true;
	this->updateSpd = 2;
}

void SensorThread::run() {
	if (!ctx) return; //dont have a context, dont run.
	psvr_sensor_frame frame;

	while (running) {
		psvr_read_sensor_sync(ctx, (uint8_t *)&frame, sizeof(psvr_sensor_frame));
		emit frameUpdate(&frame);

		switch (this->updateSpd) {
		default:
			yieldCurrentThread();
			break;
		case 1:
			msleep(500); //0.5 seconds
			break;
		case 2:
			msleep(1000); //1 second
			break;
		case 3:
			msleep(2000); //2 seconds
			break;
		case 4:
			msleep(3000); //3 seconds
			break;
		}
	}

	this->exit(0);
}

void SensorThread::setUpdateSpeed(int spd) { this->updateSpd = spd; }
void SensorThread::stopThread() { this->running = false; }
//---------------------------------------------


//---------------------------------------------
//Control Thread
//---------------------------------------------
ControlThread::ControlThread(struct psvr_context *ctx) {
	this->ctx = ctx;
	this->setTerminationEnabled(true);
}

void ControlThread::run() {
	if (!ctx) return; //dont have a context, dont run.
	psvr_control_frame frame;

	forever {
		psvr_read_control_sync(ctx, (uint8_t*)&frame, sizeof(psvr_control_frame));
		emit frameUpdate(&frame);
	}

	this->exit(0);
} 
void ControlThread::stopThread() {
	this->terminate();
}
//---------------------------------------------

//---------------------------------------------
//Data Viewer Window
//---------------------------------------------
DataViewer::DataViewer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	ui.txt_control_log->append("Initializing.");

	ui.cmbx_update_spd->setCurrentIndex(2);

	DataViewer::loggingArea = ui.txt_control_log;

	psvr_log logger = DataViewer::psvr_logger;
	psvr_set_log(&logger);

	psvr_context *ctx = nullptr;

	int err = psvr_open(&ctx); //open psvr context

	this->ctx = ctx;

	this->sensorThread = nullptr;
	this->controlThread = nullptr;

	if (err == 0) {
		ui.txt_control_log->append("Connection Established.");

		this->sensorThread = new SensorThread(this->ctx);
		connect(
			this->ui.cmbx_update_spd,
			SIGNAL(currentIndexChanged(int)),
			this->sensorThread,
			SLOT(setUpdateSpeed(int))
		);
		connect(
			this->sensorThread,
			SIGNAL(frameUpdate(void*)),
			this,
			SLOT(sensorFrame(void*)),
			Qt::BlockingQueuedConnection
		);

		this->sensorThread->start();

		this->controlThread = new ControlThread(this->ctx);
		connect(
			this->controlThread,
			SIGNAL(frameUpdate(void*)),
			this,
			SLOT(controlFrame(void*)),
			Qt::BlockingQueuedConnection
		);

		this->controlThread->start();
	} else
		ui.txt_control_log->append("Error establishing connection.");

	//setup buttons
	connect(
		this->ui.btn_headsetOn,
		SIGNAL(clicked()),
		this,
		SLOT(headsetOn())
	);
	connect(
		this->ui.btn_headsetOff,
		SIGNAL(clicked()),
		this,
		SLOT(headsetOff())
	);
	connect(
		this->ui.btn_vrMode,
		SIGNAL(clicked()),
		this,
		SLOT(enableVRMode())
	);
	connect(
		this->ui.btn_vrMode2,
		SIGNAL(clicked()),
		this,
		SLOT(enableVRMode2())
	);
	connect(
		this->ui.btn_getDeviceInfo,
		SIGNAL(clicked()),
		this,
		SLOT(getDeviceInfo())
	);
	connect(
		this->ui.btn_processorOff,
		SIGNAL(clicked()),
		this,
		SLOT(turnProcessorOff())
	);
	connect(
		this->ui.btn_recenter,
		SIGNAL(clicked()),
		this,
		SLOT(recenter())
	);

	connect(
		this->ui.btn_send_manual,
		SIGNAL(clicked()),
		this,
		SLOT(sendManualCommand())
	);
}

DataViewer::~DataViewer() {
	psvr_set_log(nullptr);

	if (this->sensorThread) {
		this->sensorThread->stopThread();
		this->sensorThread->wait();

		disconnect(
			this->ui.cmbx_update_spd,
			SIGNAL(currentIndexChanged(int)),
			this->sensorThread,
			SLOT(setUpdateSpeed(int))
		);
		disconnect(
			this->sensorThread,
			SIGNAL(frameUpdate(void*)),
			this,
			SLOT(sensorFrame(void*))
		);

		delete this->sensorThread;
		this->sensorThread = nullptr;
	}

	if (this->controlThread) {
		this->controlThread->stopThread();
		this->controlThread->wait();

		disconnect(
			this->controlThread,
			SIGNAL(frameUpdate(void*)),
			this,
			SLOT(controlFrame(void*))
		);

		delete this->controlThread;
		this->controlThread = nullptr;
	}

	psvr_close(this->ctx); //close the context
}

QString hexToString(uint8_t x, int bytesize = 1) { return QString("0x%1").arg(x, bytesize * 2, 16, QLatin1Char('0')); }

void DataViewer::sensorFrame(void *data) {
	psvr_sensor_frame* frame = (psvr_sensor_frame*)data;

	ui.lbl_button_reserved->setText(frame->s.button.reserved ? "True" : "False");
	ui.lbl_btn_plus->setText(frame->s.button.plus ? "True" : "False");
	ui.lbl_btn_minus->setText(frame->s.button.minus ? "True" : "False");
	ui.lbl_btn_mute->setText(frame->s.button.mute ? "True" : "False");

	ui.lbl_reserved0->setText(hexToString(frame->s.reserved0));
	ui.lbl_volume->setText(QString::number(frame->s.volume));

	QString reserved1;
	for (int i = 0; i < 5; i++) {
		reserved1.append(hexToString(frame->s.reserved1[i]));
		reserved1.append(" ");
	}
	ui.lbl_reserved1->setText(reserved1);

	ui.lbl_status_worn->setText(frame->s.status.worn ? "True" : "False");
	ui.lbl_status_disp_active->setText(frame->s.status.display_active ? "False" : "True");
	ui.lbl_status_hdmi_dc->setText(frame->s.status.hdmi_disconnected ? "True" : "False");
	ui.lbl_status_mic_mute->setText(frame->s.status.microphone_muted ? "True" : "False");
	ui.lbl_status_headphone_connect->setText(frame->s.status.headphone_connected ? "True" : "False");
	ui.lbl_status_reserved->setText(frame->s.status.reserved ? "True" : "False");
	ui.lbl_status_tick->setText(frame->s.status.tick ? "True" : "False");

	QString reserved2;
	for (int i = 0; i < 11; i++) {
		reserved2.append(hexToString(frame->s.reserved2[i]));
		reserved2.append(" ");
	}
	ui.lbl_reserved2->setText(reserved2);

	ui.lbl_data1_gyro_yaw->setText(QString::number(frame->s.data[0].gyro.yaw));
	ui.lbl_data1_gyro_pitch->setText(QString::number(frame->s.data[0].gyro.pitch));
	ui.lbl_data1_gyro_roll->setText(QString::number(frame->s.data[0].gyro.roll));
	ui.lbl_data1_accel_x->setText(QString::number(frame->s.data[0].accel.x));
	ui.lbl_data1_accel_y->setText(QString::number(frame->s.data[0].accel.y));
	ui.lbl_data1_accel_z->setText(QString::number(frame->s.data[0].accel.z));

	ui.lbl_data2_gyro_yaw->setText(QString::number(frame->s.data[1].gyro.yaw));
	ui.lbl_data2_gyro_pitch->setText(QString::number(frame->s.data[1].gyro.pitch));
	ui.lbl_data2_gyro_roll->setText(QString::number(frame->s.data[1].gyro.roll));
	ui.lbl_data2_accel_x->setText(QString::number(frame->s.data[1].accel.x));
	ui.lbl_data2_accel_y->setText(QString::number(frame->s.data[1].accel.y));
	ui.lbl_data2_accel_z->setText(QString::number(frame->s.data[1].accel.z));

	QString reserved3;
	for (int i = 0; i < 12; i++) {
		reserved3.append(hexToString(frame->s.reserved3[i]));
		reserved3.append(" ");
	}
	ui.lbl_reserved3->setText(reserved3);

	QString raw;
	for (int i = 0; i < 64; i++) {
		raw.append(hexToString(frame->raw[i]));
		raw.append(" ");
	}
	ui.lbl_sensor_raw->setText(raw);
}

void DataViewer::controlFrame(void* data) {
	psvr_control_frame* frame = (psvr_control_frame*)data;

	//--------------------
	//control frame info
	//--------------------
	ui.lbl_control_id->setText(hexToString(frame->s.r_id));
	ui.lbl_control_status->setText(hexToString(frame->s.gp_id));
	ui.lbl_control_start->setText(hexToString(frame->s.start));
	ui.lbl_control_len->setText(hexToString(frame->s.length));

	QString craw;
	for (int i = 0; i < 68; i++) {
		craw.append(hexToString(frame->raw[i]));
		craw.append(" ");
	}
	ui.lbl_control_raw->setText(craw);
	//--------------------

	switch (frame->s.r_id) {
	case eRT_Info:
	{
		QString reserved0;
		for (int i = 0; i < 3; i++) {
			reserved0.append(hexToString(frame->s.dinfo.s2.reserved0[i], 2));
			reserved0.append(" ");
		}
		ui.lbl_info_reserved0->setText(reserved0);

		ui.lbl_info_major->setText(QString::number(frame->s.dinfo.s2.version.major));
		ui.lbl_info_minor->setText(QString::number(frame->s.dinfo.s2.version.minor));

		QString reserved1;
		for (int i = 0; i < 3; i++) {
			reserved1.append(hexToString(frame->s.dinfo.s2.reserved1[i]));
			reserved1.append(" ");
		}
		ui.lbl_info_reserved1->setText(reserved1);

		QString serial;
		for (int i = 0; i < 16; i++) {
			serial.append((const char*)frame->s.dinfo.s2.serialNumber[i]);
			serial.append(" ");
		}
		ui.lbl_info_serial->setText(serial);

		QString reserved2;
		for (int i = 0; i < 21; i++) {
			reserved2.append(hexToString(frame->s.dinfo.s2.reserved2[i]));
			reserved2.append(" ");
		}
		ui.lbl_info_reserved2->setText(reserved2);

		QString raw;
		for (int i = 0; i < 48; i++) {
			raw.append(hexToString(frame->s.dinfo.raw[i]));
			raw.append(" ");
		}
		ui.lbl_info_raw->setText(raw);
	}
		break;
	case eRT_Status:
	{
		ui.lbl_cStatus_headsetOn->setText(frame->s.dstatus.s.headsetOn ? "True" : "False");
		ui.lbl_cStatus_worn->setText(frame->s.dstatus.s.worn ? "True" : "False");
		ui.lbl_cStatus_cinematic->setText(frame->s.dstatus.s.cinematic ? "True" : "False");
		ui.lbl_cStatus_mReserved0->setText(frame->s.dstatus.s.maskreserved0 ? "True" : "False");
		ui.lbl_cStatus_headphones->setText(frame->s.dstatus.s.headphones ? "True" : "False");
		ui.lbl_cStatus_mute->setText(frame->s.dstatus.s.mute ? "True" : "False");
		ui.lbl_cStatus_cec->setText(frame->s.dstatus.s.cec ? "True" : "False");
		ui.lbl_cStatus_mReserved1->setText(frame->s.dstatus.s.maskreserved1 ? "True" : "False");
		ui.lbl_cStatus_vol->setText(QString::number(frame->s.dstatus.s.volume));
		ui.lbl_cStatus_reserved0->setText(hexToString(frame->s.dstatus.s.reserved0, 2));
		ui.lbl_cStatus_bridgeOutputID->setText(hexToString(frame->s.dstatus.s.bridgeOutputID));

		QString raw;
		for (int i = 0; i < 8; i++) {
			raw.append(hexToString(frame->s.dstatus.raw[i]));
			raw.append(" ");
		}
		ui.lbl_cStatus_raw->setText(raw);
	}
		break;
	case eRT_Unsolicited:
	{
		ui.lbl_report_id->setText(hexToString(frame->s.ureport.s.id));
		ui.lbl_report_code->setText(hexToString(frame->s.ureport.s.code));

		QString msg;
		for (int i = 0; i < 58; i++) {
			msg.append((const char)frame->s.ureport.s.message[i]);
			if (frame->s.ureport.s.message[i] == '\0') break;
		}
		ui.lbl_report_msg->setText(msg);
		ui.txt_control_log->append(msg);

		QString raw;
		for (int i = 0; i < 60; i++) {
			raw.append(hexToString(frame->s.ureport.raw[i]));
			raw.append(" ");
		}
		ui.lbl_report_raw->setText(raw);
	}
		break;
	}
}

//---------------------------------------------
//Button functions
//---------------------------------------------
void DataViewer::headsetOn() {
	if (!ctx) return; //dont have a context, dont run.
	ui.txt_control_log->append("Turning headset on.");
	uint32_t on = 1;
	psvr_send_command_sync(ctx, eRID_HeadsetPower, (uint8_t*)&on, 4);
}
void DataViewer::headsetOff() {
	if (!ctx) return; //dont have a context, dont run.
	ui.txt_control_log->append("Turning headset off.");
	uint32_t on = 0;
	psvr_send_command_sync(ctx, eRID_HeadsetPower, (uint8_t*)&on, 4);
}
void DataViewer::enableVRMode() {
	if (!ctx) return; //dont have a context, dont run.
	ui.txt_control_log->append("Enabling VR Mode w/tracking.");
	uint32_t payload[2];
	payload[0] = 0xFFFFFF00;
	payload[1] = 0x00000000;
	printf("Enable VR Mode\n");
	psvr_send_command_sync(ctx, eRID_VRTracking, (uint8_t *)&payload, 8);
}
void DataViewer::enableVRMode2() {
	if (!ctx) return; //dont have a context, dont run.
	ui.txt_control_log->append("Enabling VR Mode w/o tracking.");
	uint32_t on = 1;
	psvr_send_command_sync(ctx, eRID_VRMode, (uint8_t *)&on, 8);
}
/*void DataViewer::enableCinematicMode() {
	if (!ctx) return; //dont have a context, dont run.
	ui.txt_control_log->append("Enabling Cinematic Mode.<br />");
}*/
void DataViewer::getDeviceInfo() {
	if (!ctx) return; //dont have a context, dont run.
	ui.txt_control_log->append("Getting Device Info.");
	uint8_t cmd[] = { 0x80, 0, 0, 0, 0, 0, 0, 0 };
	psvr_send_command_sync(ctx, eRID_DeviceInfo, cmd, 8);
}
void DataViewer::turnProcessorOff() {
	if (!ctx) return; //dont have a context, dont run.
	ui.txt_control_log->append("Turning Processor Off.");
	uint8_t on[] = { 0x01, 0x00, 0x00, 0x00 };
	psvr_send_command_sync(ctx, eRID_ProcessorPower, on, 4);
}
void DataViewer::recenter() {
	if (!ctx) return; //dont have a context, dont run.
	ui.txt_control_log->append("Recentering.");

	uint8_t cmd[] = { 0x00, 0x00, 0x00, 0x00 };
	psvr_send_command_sync(ctx, eRID_Recenter, cmd, 4);
}

void DataViewer::sendManualCommand() {
	if (!ctx) return; //dont have a context, dont run.
	
	QString reportIDStr = ui.txt_control_reportID->text();
	QString dataStr = ui.txt_control_data->text();

	if (reportIDStr.size() == 0 || dataStr.size() == 0) {
		ui.txt_control_log->append("Empty Report ID or Data.");
		return;
	}

	//if we are in a hex format (i.e. 0xFF) remove the 0x
	if (reportIDStr.at(1) == 'x' || reportIDStr.at(1) == 'X') reportIDStr = reportIDStr.mid(2);

	QByteArray reportID = QByteArray::fromHex(reportIDStr.toLatin1());

	if (reportID.size() == 1) {
		//if we are in a hex format (i.e. 0xFF) remove the 0x
		if (dataStr.at(1) == 'x' || dataStr.at(1) == 'X') dataStr = dataStr.mid(2);

		QByteArray data = QByteArray::fromHex(dataStr.toLatin1());

		ui.txt_control_log->append("Sending command.");
		psvr_send_command_sync(
			ctx,
			(uint8_t)reportID.data()[0],
			(uint8_t*)data.data(),
			(uint32_t)data.size()
		);
	} else {
		ui.txt_control_log->append("Report ID is not one byte!");
	}

	/*
	QByteArray array1;
	array1.resize(2);
	array1[0] = 0xFF;
	array1[1] = 0xFF;

	QString str = "0xFFFF";
	QString value =  str.mid(2);    // "FFFF"   <- just the hex values!
	QByteArray array2 = QByteArray::fromHex(value.toLatin1());

	qDebug() << array1;             // not printable chars
	qDebug() << array2;

	qDebug() << array1.toHex();     // to have a printable form use "toHex()"!
	qDebug() << array2.toHex();
	*/
}
//---------------------------------------------

//---------------------------------------------
//Logging stuff
//---------------------------------------------
QTextEdit* DataViewer::loggingArea = nullptr;

void DataViewer::psvr_logger(const char * msg, va_list args) {
	char buff[256];
	vsnprintf(buff, 256, msg, args);

	DataViewer::loggingArea->append(buff);
}
//---------------------------------------------