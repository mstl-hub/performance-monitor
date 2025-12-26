package components

import (
	"pm/gui/subcomponents"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/widget"
)

type NavMenu struct {
}

func NewNavMenuContainer() *fyne.Container {
	menuItems := createNavMenuItems()

	list := widget.NewList(
		func() int { return len(menuItems) },
		func() fyne.CanvasObject {
			return subcomponents.NewIconLabel(resourcePerformanceMonitorIco, "")
		},
		func(lii widget.ListItemID, co fyne.CanvasObject) {
			iconLabel := co.(*subcomponents.IconLabel)
			iconLabel.Icon.SetResource(menuItems[lii].Icon.Resource)
			iconLabel.Label.SetText(menuItems[lii].Label.Text)
		})

	container := container.NewBorder(nil, nil, nil, nil, list)
	// container.Resize(fyne.NewSize(128, float32(64*list.Length())))

	return container
}

func createNavMenuItems() []*subcomponents.IconLabel {
	status := subcomponents.NewIconLabel(resourcePerformanceMonitorIco, "Status")
	themes := subcomponents.NewIconLabel(resourcePerformanceMonitorIco, "Themes")
	monitor := subcomponents.NewIconLabel(resourcePerformanceMonitorIco, "Monitor")
	stream := subcomponents.NewIconLabel(resourcePerformanceMonitorIco, "Stream")
	settings := subcomponents.NewIconLabel(resourcePerformanceMonitorIco, "Settings")

	items := []*subcomponents.IconLabel{status, themes, monitor, stream, settings}
	return items
}
