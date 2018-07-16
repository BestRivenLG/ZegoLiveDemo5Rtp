#include "ZegoRoomList.h"

ZegoRoomList::ZegoRoomList(QWidget *parent) : QTableView(parent)
{
	
}

ZegoRoomList::~ZegoRoomList()
{

}

void ZegoRoomList::initRoomList()
{
	//�����п��ɱ�
	this->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
	this->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
	this->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
	this->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
	//����ÿ�еĿ��
	this->setColumnWidth(0, 310);
	this->setColumnWidth(1, 100);
	this->setColumnWidth(2, 110);
	this->setColumnWidth(3, 120);
	//������ͷ
	this->verticalHeader()->setVisible(false);
	//��ͷ���ݿ���
	this->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	//��ͷ���ɵ��
	this->horizontalHeader()->setSectionsClickable(false);
	//�����ޱ߿�
	this->setFrameShape(QFrame::NoFrame);
	//�б����ݲ��ɱ༭
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//����ѡ��ʱΪ����ѡ��        
	this->setSelectionBehavior(QAbstractItemView::SelectRows);
	//���itemʱȥ�����߿�
	this->setItemDelegate(new NoFocusFrameDelegate(this));
	//Ĭ�����һ��ռ�����µ�λ��
	this->horizontalHeader()->setStretchLastSection(true);
	//����ˮƽ������
	this->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal{height: 0px;}");
	this->horizontalScrollBar()->setVisible(false);
	//StyleSheet
	this->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{"
		"background: transparent;"
		"width: 9px;"
		"margin: 0px 0px 2px 0px;}"
		"QScrollBar::handle:vertical{"
		"background: rgb(195, 195, 195);"
		"min-height: 20px;"
		"border-radius: 3px;}"
		"QScrollBar::handle:vertical:hover{"
		"background:rgba(0, 0, 0, 30%);}"
		"QScrollBar::add-line:vertical{"
		"height: 0px;"
		"subcontrol-position: bottom;"
		"subcontrol-origin: margin; }"
		"QScrollBar::sub-line:vertical{"
		"height: 0px;"
		"subcontrol-position: top;"
		"subcontrol-origin: margin; }");

}