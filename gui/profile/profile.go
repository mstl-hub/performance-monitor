package profile

import "pm/device"

type Profiles struct {
	Profiles      []*Profile
	selectedIndex int
}

func LoadProfiles() *Profiles {
	p := &Profiles{}

	return p
}

func (p *Profiles) Selected() *Profile {
	if p.selectedIndex >= len(p.Profiles) {
		return p.Profiles[0]
	}
	return p.Profiles[p.selectedIndex]
}

func (p *Profiles) SelectedIndex() int {
	return p.selectedIndex
}

func (p *Profiles) SaveSelected() {
	panic("unimplemented")
}

type Profile struct {
	Name        string
	Image       []byte
	DeviceTheme device.DeviceTheme
}

func LoadProfile() *Profile {
	p := &Profile{}

	return p
}
