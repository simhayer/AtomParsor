from tkinter import*
from tkinter import filedialog
from tkinter import messagebox
#import tkMessageBox as messagebox

root = Tk()
root.geometry("400x450")

def on_closing():
    if messagebox.askokcancel("Quit", "Do you want to quit?"):
        root.destroy()

root.protocol("WM_DELETE_WINDOW", on_closing)

##root.filename = filedialog.askopenfilename(initialdir='./bin', title="Select a file",filetypes=(("KML files","*.kml"),("all files","*.*")))

def printPoint(r,j):
    ##POINT FRAME
    pointFrame = Frame(disp,highlightbackground="black", highlightthickness=2,padx=1,pady=1)
    pointFrame.grid(row=r,column=j,sticky=W+E,columnspan=3,padx=1,pady=1)

    nameField = Label(pointFrame, text="Name")
    nameField.grid(row = 0, column = 0)

    pointName = Entry(pointFrame)
    pointName.grid(row = 0, column = 1,sticky=W+E)

    CoordFrame = Frame(pointFrame)
    CoordFrame.grid(row = 1, column = 0,columnspan=3,sticky=W+E)

    coordLongField = Label(CoordFrame, text="Longitude",width=12)
    coordLongField.grid(row = 0, column = 0)

    coordLong = Label(CoordFrame, text="0.0000000")
    coordLong.grid(row = 1, column = 0)


    coordLatField = Label(CoordFrame, text="Latitude",width=12)
    coordLatField.grid(row = 0, column = 1)

    coordLat = Label(CoordFrame, text="0.0000000")
    coordLat.grid(row = 1, column = 1)


    coordAltField = Label(CoordFrame, text="Altitude",width=12)
    coordAltField.grid(row = 0, column = 2)

    coordAlt = Label(CoordFrame, text="0.0000000")
    coordAlt.grid(row = 1, column = 2)

def printPath(r,j):
    ##PATH FRAMEEE
    pathFrame = Frame(disp,highlightbackground="black", highlightthickness=2,padx=1,pady=1)
    pathFrame.grid(row=r,column=j,columnspan=3,sticky=W+E,padx=1,pady=1)

    nameFieldPath = Label(pathFrame, text="Name")
    nameFieldPath.grid(row = 0, column = 0)

    pathName = Entry(pathFrame)
    pathName.grid(row = 0, column = 1)


    pathLengthField = Label(pathFrame, text="Length")
    pathLengthField.grid(row = 1, column = 0)

    pathLength = Label(pathFrame, text="0.0000000")
    pathLength.grid(row = 2, column = 0)


    isLoopField = Label(pathFrame, text="isLoop")
    isLoopField.grid(row = 1, column = 1)

    isLoop = Label(pathFrame, text="0.0000000")
    isLoop.grid(row = 2, column = 1)

def printStyle(r,j):
    ##Style Frame
    styleFrame = Frame(disp,highlightbackground="black", highlightthickness=2,padx=2,pady=2)
    styleFrame.grid(row=r,column=j,columnspan=3,sticky=W+E,padx=1,pady=1)


    colourField = Label(styleFrame, text="Colour")
    colourField.grid(row = 0, column = 0)

    colour = Entry(styleFrame)
    colour.grid(row = 1, column = 0,padx=2,pady=2)


    widthField = Label(styleFrame, text="Width")
    widthField.grid(row = 0, column = 1)

    width = Entry(styleFrame)
    width.grid(row = 1, column = 1,padx=2,pady=2)


    fillField = Label(styleFrame, text="Fill")
    fillField.grid(row = 0, column = 2)

    fill = Label(styleFrame, text="00000")
    fill.grid(row = 1, column = 2)


def open():
    filetypes = (
        ('KML files', '*.kml'),
        ('All files', '*.*')
    )

    filename = filedialog.askopenfilename(
    title='Open a file',
    initialdir='./bin',
    filetypes=filetypes)

    ##opened filename saved in filename

def about():
    aboutFrame = Toplevel()
    aboutName = Label(aboutFrame,text="Author: Simrat Hayer")
    aboutName.grid(row=0,column=0)
    aboutCourse = Label(aboutFrame,text="Course: Cis2750")
    aboutCourse.grid(row=1,column=0)
    aboutInst = Label(aboutFrame,text="                 Instructor: Dennis Nikitenko             ")
    aboutInst.grid(row=2,column=0)
    

##MENU
menuOptions = [
    "File",
    "Open (Ctrl + O)",
    "Save (Ctrl + S)",
    "Save As",
    "About",
    "Exit (Ctrl + X)"
]


menuCur = StringVar()
menuCur.set(menuOptions[0])


selected = StringVar()
def menuFunc(d):
    selected = menuCur.get()
    if selected == "Open (Ctrl + O)":
        open()
    
    if selected == "Exit (Ctrl + X)":
        on_closing()

    if selected == "About":
        about()

menu = OptionMenu(root, menuCur, *menuOptions,command=menuFunc)
menu.grid(row = 0, column = 0,columnspan=1, sticky=W+E)


Middle = Frame(root,highlightbackground="blue", highlightthickness=2,padx=5,pady=5)
Middle.grid(row=1,column=0, sticky = W+E,padx=5,pady=5)

v = Scrollbar(Middle, orient='vertical')
v.grid(row=0,column=1,padx=5,pady=5,sticky=N+S)
#v.config(command=YView)

canvas = Canvas(Middle,height=200, width=310,scrollregion=(0,0,500,500))
v.config(command=canvas.yview)
canvas.config(yscrollcommand=v.set)
canvas.grid(row=0,column=0, sticky = W+E,padx=5,pady=5)

disp = Frame(canvas)

canvas.create_window((10,10), window=disp, anchor="nw")


# v.config(command=YView)
# v.grid(row=0,column=3)


# empty = Label(disp, text="                                     ",relief=SUNKEN)
# empty.grid(row = 0, column = 0,columnspan=3,sticky=W+E)
pointHead = Label(disp, text="Point")
pointHead.grid(row = 0, column = 0)

printPoint(1,0)
printPoint(2,0)

pathHead = Label(disp, text="Path")
pathHead.grid(row = 3, column = 0)

printPath(4,0)

styleHead = Label(disp, text="Style")
styleHead.grid(row = 5, column = 0)

printStyle(6,0)

#LOG 
Bottom = Frame(root,highlightbackground="blue", highlightthickness=2,padx=5,pady=5)
Bottom.grid(row=2,column=0, sticky = W+E,padx=5,pady=5)

v1 = Scrollbar(Bottom, orient='vertical')
v1.grid(row=0,column=1,padx=5,pady=5,sticky=N+S)

canvas1 = Canvas(Bottom,height=100, width=300,scrollregion=(0,0,500,500))
#v1.config(command=canvas1.yview)
#canvas1.config(yscrollcommand=v1.set)
canvas1.grid(row=0,column=0, sticky = W+E,padx=5,pady=5)

#disp = Frame(canvas1)
log = Text(canvas1,height=5,width=35, bg="white",yscrollcommand=v1.set)
v1.config(command=log.yview)
log.insert(END,"know that the Scrollbarframe works with other widgets it is just that the dynamic component is causing issues. When I shrink the vertical size of the window past the original location of the createWidgets button, the scrollbar appears, but the scrollbar is not present for the rest of the dynamically created widgets. Does the canvas not detect that the vertical size of the frame increases with a button press")
log.config(state=DISABLED)
log.grid(row=0,column=0, sticky = W+E,padx=5,pady=5)

canvas1.create_window((10,10), window=log, anchor="nw")

def openShortcut(event=None):
    open()

def exitShortcut(event=None):
    on_closing()

root.bind('<Control-o>', openShortcut)
root.bind('<Control-x>', exitShortcut)

root.mainloop()
