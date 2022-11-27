TRANSLATIONS    = english.ts




TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on


HEADERS	+= qlgapplication.h \
	qlgmainwindow.h \
	qlgoutputwindow.h \
	qlgcalculatordlgimpl.h \
	qlgnumber.h \
	qcompletionedit.h \
	qlgprojectmanager.h \
	qlgtruetable.h \
	qlgfunctionstable.h \
	qlgquine.h \
	qlgcircuitview.h \
	qlgcircuitline.h \
	qlgcircuitcomponent.h \
	qlgtimechartwindow.h \
	qlgsignalgeneratordlgimpl.h \
	qlgcircuitwindow.h \
	qlgfsmdlgimpl.h \
	qlgfsmwindow.h \
	qlgfsmview.h \
	qlgaddstatedlgimpl.h \
	qlgaddtransitiondlgimpl.h \
	qlgupdatetransitiondlgimpl.h \
	qlgfsmsimulationdlgimpl.h \
	qlgtransition.h \
	qlgstate.h \
	qlgtitle.h \
	qlgcircuit.h \
	qlgtruetableview.h \
	qlgboolfunction.h \
	qlgfsmmachine.h \
	qlgfsmoptions.h \
	qlgcircuitcomponentdlgimpl.h \
	qlgcircuitdlgimpl.h \
	qlgoutputtable.h \
	qlgoutp.h \
	qlgtimechartoptionsdlgimpl.h
SOURCES	+= main.cpp \
	qlgapplication.cpp \
	qlgmainwindow.cpp \
	qlgoutputwindow.cpp \
	qlgcalculatordlgimpl.cpp \
	qlgnumber.cpp \
	qcompletionedit.cpp \
	qlgprojectmanager.cpp \
	qlgtruetable.cpp \
	qlgfunctionstable.cpp \
	qlgquine.cpp \
	qlgcircuitview.cpp \
	qlgcircuitline.cpp \
	qlgcircuitcomponent.cpp \
	qlgtimechartwindow.cpp \
	qlgsignalgeneratordlgimpl.cpp \
	qlgcircuitwindow.cpp \
	qlgfsmdlgimpl.cpp \
	qlgfsmwindow.cpp \
	qlgfsmview.cpp \
	qlgaddstatedlgimpl.cpp \
	qlgaddtransitiondlgimpl.cpp \
	qlgupdatetransitiondlgimpl.cpp \
	qlgfsmsimulationdlgimpl.cpp \
	qlgtransition.cpp \
	qlgstate.cpp \
	qlgtitle.cpp \
	qlgcircuit.cpp \
	qlgtruetableview.cpp \
	qlgboolfunction.cpp \
	qlgfsmmachine.cpp \
	qlgfsmoptions.cpp \
	qlgcircuitcomponentdlgimpl.cpp \
	qlgcircuitdlgimpl.cpp \
	qlgoutputtable.cpp \
	qlgoutp.cpp \
	qlgtimechartoptionsdlgimpl.cpp
FORMS	= qlgcalculatordlg.ui \
	qlgsignalgeneratordlg.ui \
	qlgaddstatedlg.ui \
	qlgaddtransitiondlg.ui \
	qlgupdatetransitiondlg.ui \
	qlgfsmsimulationdlg.ui \
	qlgfsmdlg.ui \
	qlgcircuitdlg.ui \
	qlgcircuitcomponentdlg.ui \
	qlgtimechartoptionsdlg.ui
IMAGES	= images/bases \
	images/bground \
	images/circuit \
	images/copy \
	images/cut \
	images/delete \
	images/editcopy \
	images/editcut \
	images/editpaste \
	images/entrada \
	images/filenew \
	images/fileopen \
	images/filesave \
	images/functions \
	images/getfsmtable \
	images/grid \
	images/helpabout \
	images/helpcontents \
	images/jkff \
	images/linea \
	images/machine \
	images/machinesimulate \
	images/nand \
	images/newcircuit \
	images/newmachine \
	images/node \
	images/nor \
	images/not \
	images/ocircuit \
	images/or \
	images/paste \
	images/personalizable \
	images/pointer \
	images/print \
	images/qlogico \
	images/rcircuit \
	images/redo \
	images/salida \
	images/signal \
	images/splash \
	images/state \
	images/table \
	images/tipo1 \
	images/tipo2 \
	images/tipo3 \
	images/tipo4 \
	images/tipo5 \
	images/transition \
	images/ttorientation \
	images/undo \
	images/xnor \
	images/xor \
	images/and \
	images/time \
	images/timeoptions

