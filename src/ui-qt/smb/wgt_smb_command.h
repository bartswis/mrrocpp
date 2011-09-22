#ifndef WGT_SMB_COMMAND_H
#define WGT_SMB_COMMAND_H

#include <QtGui/QWidget>
#include <QVBoxLayout>
#include <QDockWidget>
#include "ui_wgt_smb_command.h"
#include "../base/wgt_base.h"
#include <QTimer>

namespace mrrocpp {
namespace ui {
namespace common {
class Interface;
class UiRobot;
}
namespace smb {
class UiRobot;
const std::string WGT_SMB_COMMAND = "WGT_SMB_COMMAND";
}
}
}

class wgt_smb_command : public wgt_base
{
Q_OBJECT

public:
	wgt_smb_command(QString _widget_label, mrrocpp::ui::common::Interface& _interface, mrrocpp::ui::common::UiRobot *_robot, QWidget *parent =
			0);

private:
	Ui::wgt_smb_commandClass ui;


	// buttons callbacks

	void on_pushButton_fl_execute_clicked();
	void on_pushButton_m_execute_clicked();
	void on_pushButton_execute_all_clicked();

	void on_pushButton_read_clicked();
	void on_pushButton_ml_copy_clicked();
	void on_pushButton_ms_copy_clicked();
	void on_pushButton_ml_left_clicked();
	void on_pushButton_ml_rigth_clicked();
	void on_pushButton_ms_left_clicked();
	void on_pushButton_ms_rigth_clicked();

};

#endif // WGT_SMB_COMMAND_H
