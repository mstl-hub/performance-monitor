package components

import (
	"pm/gui/subcomponents"
	"pm/gui/themes"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/theme"
	"fyne.io/fyne/v2/widget"
)

type NavMenu struct {
	Container     *fyne.Container
	ContentWindow *ContentWindow
}

func NewNavMenu(contentWindow *ContentWindow) *NavMenu {
	navMenu := &NavMenu{}
	navMenu.ContentWindow = contentWindow

	menuItems := createNavMenuItems()

	list := widget.NewList(
		func() int { return len(menuItems) },
		func() fyne.CanvasObject {
			return subcomponents.NewIconLabel(theme.Current().Icon(themes.IconPerformanceMonitor), "")
		},
		func(lii widget.ListItemID, co fyne.CanvasObject) {
			iconLabel := co.(*subcomponents.IconLabel)
			iconLabel.Icon.SetResource(menuItems[lii].Icon.Resource)
			iconLabel.Label.SetText(menuItems[lii].Label.Text)
		})

	list.OnSelected = func(id widget.ListItemID) {
		navMenu.ContentWindow.ShowComponent(id)
	}

	container := container.NewBorder(nil, nil, nil, nil, list)

	navMenu.Container = container

	return navMenu
}

func createNavMenuItems() []*subcomponents.IconLabel {
	status := subcomponents.NewIconLabel(theme.Current().Icon(themes.IconPerformanceMonitor), "Status")
	customize := subcomponents.NewIconLabel(theme.Current().Icon(themes.IconPerformanceMonitor), "Customize")
	monitor := subcomponents.NewIconLabel(theme.Current().Icon(themes.IconPerformanceMonitor), "Monitor")
	stream := subcomponents.NewIconLabel(theme.Current().Icon(themes.IconPerformanceMonitor), "Stream")
	settings := subcomponents.NewIconLabel(theme.Current().Icon(themes.IconPerformanceMonitor), "Settings")

	items := []*subcomponents.IconLabel{status, customize, monitor, stream, settings}
	return items
}
