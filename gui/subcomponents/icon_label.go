package subcomponents

import (
	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/widget"
)

type IconLabel struct {
	widget.BaseWidget

	Icon  *widget.Icon
	Label *widget.Label
}

func NewIconLabel(resource fyne.Resource, text string) *IconLabel {
	iconLabel := &IconLabel{}
	iconLabel.Icon = widget.NewIcon(resource)
	iconLabel.Label = widget.NewLabel(text)
	iconLabel.ExtendBaseWidget(iconLabel)

	iconLabel.Icon.Resize(fyne.NewSize(16, 16))

	return iconLabel
}

func (iconLabel *IconLabel) CreateRenderer() fyne.WidgetRenderer {
	container := container.NewHBox(iconLabel.Icon, iconLabel.Label)
	return widget.NewSimpleRenderer(container)
}
