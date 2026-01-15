package components

import (
	"pm/device"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/container"
)

type PreviewWindow struct {
	Device          *device.Device
	Container       *fyne.Container
}

func NewPreviewWindow(device *device.Device) *PreviewWindow {
	pw := &PreviewWindow{}
	pw.Device = device

	container := container.NewBorder(nil, nil, nil, nil, nil)
	pw.Container = container

	return pw
}