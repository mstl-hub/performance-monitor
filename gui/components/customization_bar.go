package components

import (
	"pm/device"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/container"
)

type CustomizationBar struct {
	Device    *device.Device
	Container *fyne.Container
}

func NewCustomizationBar(device *device.Device) *CustomizationBar {
	cb := &CustomizationBar{}
	cb.Device = device

	container := container.NewBorder(nil, nil, nil, nil, nil)
	cb.Container = container

	return cb
}
