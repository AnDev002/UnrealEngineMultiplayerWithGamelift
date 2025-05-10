// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TCOSSystemsWidget.h"

void UTCOSSystemsWidget::SetWidgetController(UWidgetController* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}
