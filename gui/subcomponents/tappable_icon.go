package subcomponents

import (
	"image/color"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/canvas"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/driver/desktop"
	"fyne.io/fyne/v2/widget"
)

type TappableIcon struct {
	widget.BaseWidget
	Icon *widget.Icon
	rectangle *canvas.Rectangle
	HoverColor *color.RGBA
	BgColor *color.RGBA
	OnTapped func()
}

func NewTappableIcon(resource fyne.Resource) *TappableIcon {
	ti := &TappableIcon{}
	ti.HoverColor = &color.RGBA{255, 255, 255, 65}
	ti.BgColor = &color.RGBA{0, 0, 0, 0}
	ti.Icon = widget.NewIcon(resource)
	ti.rectangle = canvas.NewRectangle(ti.BgColor)
	ti.ExtendBaseWidget(ti)

	return ti
}

func (ti *TappableIcon) CreateRenderer() fyne.WidgetRenderer {
	container := container.NewPadded(container.NewStack(ti.rectangle, ti))
	return widget.NewSimpleRenderer(container)
}

func (ti *TappableIcon) Tapped(*fyne.PointEvent) {
	if ti.OnTapped != nil {
		ti.OnTapped()
	}
}

func (ti *TappableIcon) MouseIn(*desktop.MouseEvent) {
	ti.rectangle.FillColor = ti.HoverColor
}
func (ti *TappableIcon) MouseMoved(*desktop.MouseEvent) {}
func (ti *TappableIcon) MouseOut() {
	ti.rectangle.FillColor = ti.BgColor
}