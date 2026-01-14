package components

import (
	"net/url"
	"pm/device"
	"pm/gui/themes"
	"time"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/canvas"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/lang"
	"fyne.io/fyne/v2/theme"
	"fyne.io/fyne/v2/widget"
)

type StatusWindow struct {
	Device               *device.Device
	Container            *fyne.Container
	statusIcon           *canvas.Image
	statusText           *canvas.Text
	connectButton        *widget.Button
	learnMoreButton      *widget.Button
	connectionFailedText *canvas.Text
}

func NewStatusWindow(device *device.Device) *StatusWindow {
	sw := &StatusWindow{}
	sw.Device = device
	sw.Container = sw.createContainer()
	sw.UpdateStatus()

	return sw
}

func (sw *StatusWindow) createContainer() *fyne.Container {
	statusIcon := &canvas.Image{}
	statusText := &canvas.Text{}
	connectButton := &widget.Button{}
	learnMoreButton := &widget.Button{}
	connectionFailedText := &canvas.Text{}

	statusIcon.FillMode = canvas.ImageFillContain
	statusIcon.SetMinSize(fyne.NewSize(64, 128))

	statusText.TextSize = theme.Current().Size(themes.H4)
	statusText.Alignment = fyne.TextAlignCenter

	connectButton.OnTapped = func() {
		sw.Device.IsConnected = sw.Device.AttempConnection()
		sw.UpdateStatus()
		if !sw.Device.IsConnected {
			connectionFailedText.Show()
			time.AfterFunc(time.Second*15, func() {
				fyne.DoAndWait(func() {
					connectionFailedText.Hide()
				})
			})
		}
	}
	connectButton.Text = lang.L("Connect")
	connectButton.Alignment = widget.ButtonAlignCenter

	learnMoreButton.OnTapped = func() {
		url, err := url.Parse("https://ih1.redbubble.net/image.5272806493.7260/fposter,small,wall_texture,product,750x1000.jpg")
		if err != nil {
			return
		}
		fyne.CurrentApp().OpenURL(url)
	}
	learnMoreButton.Text = lang.L("Learn more")
	learnMoreButton.Alignment = widget.ButtonAlignCenter

	connectionFailedText.TextSize = theme.TextSize()
	connectionFailedText.Color = theme.Current().Color(themes.ColorError, fyne.CurrentApp().Settings().ThemeVariant())
	connectionFailedText.Alignment = fyne.TextAlignCenter
	connectionFailedText.Text = lang.L("Couldn't connect to a device")

	sw.statusIcon = statusIcon
	sw.statusText = statusText
	sw.connectButton = connectButton
	sw.learnMoreButton = learnMoreButton
	sw.connectionFailedText = connectionFailedText

	containerButtons := container.NewCenter(container.NewPadded(container.NewHBox(container.NewPadded(connectButton), container.NewPadded(learnMoreButton))))
	mainContainer := container.NewVBox(statusIcon, statusText, containerButtons, connectionFailedText)

	return container.NewCenter(container.NewPadded(mainContainer))
}

func (sw *StatusWindow) UpdateStatus() {
	if sw.Device.IsConnected {
		sw.statusIcon.Resource = theme.Current().Icon(themes.IconShieldCheck)
		sw.statusText.Text = lang.L("Device is connected")
		sw.statusText.Color = theme.Current().Color(themes.ColorSuccess, fyne.CurrentApp().Settings().ThemeVariant())
		sw.connectionFailedText.Hide()
	} else {
		sw.statusIcon.Resource = theme.Current().Icon(themes.IconShieldX)
		sw.statusText.Text = lang.L("Device isn't connected")
		sw.statusText.Color = theme.Current().Color(themes.ColorError, fyne.CurrentApp().Settings().ThemeVariant())
		sw.connectionFailedText.Hide()
	}
	sw.Container.Refresh()
}
