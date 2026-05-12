#include "GridWidget.h"
#include "Components/TextBlock.h"

void UGridWidget::ChangeColor(FSlateColor Color)
{
	Count->SetColorAndOpacity(Color);	
}

void UGridWidget::GridWidgetClicked()
{
	OnGridWidgetClicked.Broadcast(ItemName);
}

