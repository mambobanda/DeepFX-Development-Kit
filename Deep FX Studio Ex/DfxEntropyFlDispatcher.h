#pragma once

#include <QObject>
#include <FlDispatcher.h>

class DfxEntropyFlDispatcher : public QObject, public fl::Dispatcher {
	Q_OBJECT
public:
	void valueChanged(fl::Any *any) override;

signals:
	void valueHasChanged();
};