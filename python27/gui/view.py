from Tkinter import Tk, Label, Button, Entry, IntVar, END, W, E, Scale, Checkbutton

class Controls:

    def __init__(self, profile):
        self.profile = profile

        self.show_rgb_mask = False
        self.show_hsv_mask = False


    def start(self,master):

        self.master = master
        master.title("Controls")

        self.rgb_red_min = Scale(master, from_=0, to=255, background="red")
        self.rgb_red_max = Scale(master, from_=0, to=255, background="red")

        self.hue_label = Label(master, text="hue", background="white")
        self.sat_label = Label(master, text="sat", background="white")
        self.val_label = Label(master, text="val", background="white")

        self.rgb_green_min = Scale(master, from_=0, to=255, background="green")
        self.rgb_green_max = Scale(master, from_=0, to=255, background="green")

        self.rgb_blue_min = Scale(master, from_=0, to=255, background="blue")
        self.rgb_blue_max = Scale(master, from_=0, to=255, background="blue")

        self.hsv_hue_min = Scale(master, from_=0, to=255, background="white")
        self.hsv_hue_max = Scale(master, from_=0, to=255, background="white")

        self.hsv_sat_min = Scale(master, from_=0, to=255, background="white")
        self.hsv_sat_max = Scale(master, from_=0, to=255, background="white")

        self.hsv_val_min = Scale(master, from_=0, to=255, background="white")
        self.hsv_val_max = Scale(master, from_=0, to=255, background="white")

        # set default values
        self.rgb_red_min.set(self.profile.red[0])
        self.rgb_red_max.set(self.profile.red[1])

        self.rgb_green_min.set(self.profile.green[0])
        self.rgb_green_max.set(self.profile.green[1])

        self.rgb_blue_min.set(self.profile.blue[0])
        self.rgb_blue_max.set(self.profile.blue[1])

        self.hsv_hue_min.set(self.profile.hsv_hue[0])
        self.hsv_hue_max.set(self.profile.hsv_hue[1])

        self.hsv_sat_min.set(self.profile.hsv_sat[0])
        self.hsv_sat_max.set(self.profile.hsv_sat[1])

        self.hsv_val_min.set(self.profile.hsv_val[0])
        self.hsv_val_max.set(self.profile.hsv_val[1])

        self.show_rgb_mask_var = IntVar()
        self.show_rgb_mask_check = Checkbutton(master, text="RGB Mask",
                                               variable=self.show_rgb_mask_var)

        self.show_hsv_mask_var = IntVar()
        self.show_hsv_mask_check = Checkbutton(master, text="HSV Mask",
                                               variable=self.show_hsv_mask_var)

        # LAYOUT
        self.rgb_red_min.grid(row=0, column=0)
        self.rgb_red_max.grid(row=0, column=1)
        self.hue_label.grid(row=1,column=0,columnspan=2)
        self.sat_label.grid(row=1,column=2,columnspan=2)
        self.val_label.grid(row=1,column=4,columnspan=2)


        self.rgb_green_min.grid(row=0, column=2)
        self.rgb_green_max.grid(row=0, column=3)

        self.rgb_blue_min.grid(row=0, column=4)
        self.rgb_blue_max.grid(row=0, column=5)

        self.hsv_hue_min.grid(row=2, column=0)
        self.hsv_hue_max.grid(row=2, column=1)

        self.hsv_sat_min.grid(row=2, column=2)
        self.hsv_sat_max.grid(row=2, column=3)

        self.hsv_val_min.grid(row=2, column=4)
        self.hsv_val_max.grid(row=2, column=5)

        self.show_rgb_mask_check.grid(row=3, column=0)
        self.show_hsv_mask_check.grid(row=3, column=1)

    def update_profile(self):

        self.rgb_red_min.set(min( self.rgb_red_min.get(), self.rgb_red_max.get()))
        self.rgb_red_max.set(max( self.rgb_red_min.get(), self.rgb_red_max.get()))


        self.profile.red = [self.rgb_red_min.get(),
                            self.rgb_red_max.get()]

        self.profile.green = [self.rgb_green_min.get(),
                             self.rgb_green_max.get()]

        self.profile.blue = [self.rgb_blue_min.get(),
                              self.rgb_blue_max.get()]

        self.profile.hsv_hue = [self.hsv_hue_min.get(),
                             self.hsv_hue_max.get()]

        self.profile.hsv_sat = [self.hsv_sat_min.get(),
                             self.hsv_sat_max.get()]

        self.profile.hsv_val = [self.hsv_val_min.get(),
                             self.hsv_val_max.get()]

        self.show_rgb_mask = self.show_rgb_mask_var.get() == 1
        self.show_hsv_mask = self.show_hsv_mask_var.get() == 1
