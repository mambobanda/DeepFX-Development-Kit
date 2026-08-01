#include "DeepFXNodeEditor.h"
#include "DfxQNodeView.h"

DeepFXNodeEditor::DeepFXNodeEditor(QWidget *parent)
	: QMainWindow(parent)
{
	this->resize(500, 400);

	DfxQNodeView *view = new DfxQNodeView;

	this->setCentralWidget(view);
}
