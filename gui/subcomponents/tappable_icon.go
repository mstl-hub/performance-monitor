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
	Icon      *widget.Icon
	rectangle  *canvas.Rectangle
	HoverColor *color.RGBA
	BgColor    *color.RGBA
	OnTapped   func()
}

func NewTappableIcon(resource fyne.Resource) *TappableIcon {
	ti := &TappableIcon{}
	ti.OnTapped = nil
	ti.HoverColor = &color.RGBA{255, 255, 255, 25}
	ti.BgColor = &color.RGBA{0, 0, 0, 0}
	ti.Icon = widget.NewIcon(resource)
	ti.rectangle = canvas.NewRectangle(ti.BgColor)
	ti.rectangle.SetMinSize(fyne.NewSize(ti.Icon.MinSize().Width*1.25, ti.Icon.MinSize().Height*1.25))
	ti.rectangle.CornerRadius = ti.rectangle.MinSize().Width / 10
	ti.ExtendBaseWidget(ti)

	return ti
}

func (ti *TappableIcon) CreateRenderer() fyne.WidgetRenderer {
	container := container.NewPadded(container.NewStack(ti.rectangle, container.NewCenter(ti.Icon)))
	return widget.NewSimpleRenderer(container)
}

func (ti *TappableIcon) Tapped(*fyne.PointEvent) {
	if ti.OnTapped != nil {
		ti.OnTapped()
	}
}

func (ti *TappableIcon) MouseIn(*desktop.MouseEvent) {
	ti.rectangle.FillColor = ti.HoverColor
	ti.rectangle.Refresh()
}
func (ti *TappableIcon) MouseMoved(*desktop.MouseEvent) {}
func (ti *TappableIcon) MouseOut() {
	ti.rectangle.FillColor = ti.BgColor
	ti.rectangle.Refresh()
}
