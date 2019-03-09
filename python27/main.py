import cv2

import yellow_profile
from cameras import logitech_c270, generic
from pipelines import cube_tracker

from gui.view import Controls

VIDEO_SOURCE_NUMBER = 0

from Tkinter import Tk

def video(debug=False):

    cap = cv2.VideoCapture(VIDEO_SOURCE_NUMBER)

    root = Tk()
    controls = Controls(yellow_profile)
    controls.start(root)

    while(True):

        _, frame = cap.read()

        img = cube_tracker.process(frame,
                                   controls,
                                   logitech_c270,
                                   debug)

        # use this instead of root.mainloop()
        root.update_idletasks()
        root.update()

        controls.update_profile()

        cv2.imshow('frame', img )
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

def single_frame(debug=False):

    img = cv2.imread("frc_cube.jpg")
    img = cube_tracker.process(img,
                               yellow_profile,
                               generic,
                               debug)

    cv2.imshow('Objects Detected',img)
    cv2.waitKey()


#TODO enable options to be passed from command line
if __name__ == '__main__':
    video(debug=False)
