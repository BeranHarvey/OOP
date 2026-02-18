# BEng Biomedical Engineering - Object-Oriented Programming

## Coursework 1

### Objective

To gain practical experience of C++ coding to solve a simple problem.

### Introduction

Magnetic Resonance Imaging is a powerful imaging technique capable of producing images of
internal organs with good soft tissue contrast. It relies on a number of hardware components, as
illustrated in Figure 1: a powerful permanent magnet; magnetic field gradients provided by
dedicated gradient coils; and radio-frequency coils to excite the nuclear spins
(typically 1H) and detect the resulting nuclear magnetic resonance signal.

The role of the gradient coils is to impart a linear change in the resonance frequency of the 1H
nuclear spins as a function of position, which allows the position of the signal to be inferred. To
provide full images, these magnetic field gradients need to be rapidly modulated in time, which is
done by driving very large currents through the gradient coils and switching them rapidly on &
off at appropriate times along the appropriate axes.

#### Eddy-currents in MRI

Because of Faraday’s law of electromagnetic induction, these rapid and intense changes in the
magnetic fields induce voltages, which in turn drive currents in nearby conductive structures.
These eddy currents create magnetic fields that oppose the change in magnetic field that created
them (this is called Lenz’s law).

In MRI, the net result is that the temporal evolution of the magnetic field gradient actually
produced will differ from the waveform provided to the gradient amplifier. This is illustrated in
Figure 2, showing how an ideal trapezoidal input waveform will end up distorted and lagging
behind what was expected, and the resulting impact on images acquired using echo-planar
imaging (a single-shot imaging technique). Eddy currents can affect the images in many different
ways, depending on the type of MRI sequence used and other factors, but they are invariably
problematic. For this reason, a variety of techniques are used to minimise eddy-currents,
including hardware solutions (e.g. actively shielded gradients) and gradient pre-emphasis.

#### Gradient pre-emphasis

Gradient pre-emphasis involves modifying the signal time course provided to the gradient
amplifier to compensate for the expected effect of eddy-currents, as illustrated in Figure 3. If the
properties of the eddy-currents are known, it is possible to predict the time course of the actual
gradient for a given input waveform. With pre-emphasis, the input waveform is modified so that
the time course of the actual gradient matches the desired waveform more accurately.
For the purposes of pre-emphasis, eddy-currents are typically modelled as multiple components,
each with a given amplitude and decay rate constant. The parameters of the different eddy-
current components are measured during routine maintenance as part of the calibration of the
MRI scanner, and stored on the system for use in gradient pre-emphasis.

### Instructions

Your task in this coursework is to write a C++ program to implement a simple gradient pre-
emphasis strategy. This program should perform the following main steps (these are described in
more detail below):

1. Load the parameters describing the eddy-currents for the system from the file provided.
2. Load the desired time course for the gradient from the file provided.
3. Make a copy the desired time course, which will be the initial input gradient time course.
4. Compute the predicted time course for the input time course by taking the effects of eddy-
currents into account.
5. Compute the maximum absolute deviation between the desired and predicted time
courses, and display this value on the terminal
6. Display the input and predicted time courses on the terminal.
7. Compute the difference between the desired and predicted time course.
8. Add that difference back to the current input time course.
9. Go back to step 4 and repeat for the desired number of iterations
10. If requested, write the final estimate of the compensated input time course to the file
specified on the command-line

#### Command-line interface

Your program should accept at least 2 arguments: the configuration file containing the parameters
for the eddy-currents, and the file containing the desired time course. If a third argument is
provided, this should be interpreted as a request to store the estimated compensated time course
to the file specified in that argument.

Your program should also accept a “-n num” command-line option to override the default number
of iterations. If left unspecified, the default number of iterations should be set to 10.

#### Loading the input data

You are provided with a text file called parameters.txt file containing the eddy-current
parameters. Each line in this file contains two values: the amplitude and the rate constant for each
eddy-current component.

You are also provided with a text file called gradient.txt file containing the intensity values at
regular sampling intervals for the desired gradient time course.

#### Computing the predicted gradient time course

The impact of the eddy-currents can be predicted using a simple model. Each eddy-current
component can be modelled independently, and will have an associated current 𝐼𝑛 (with initial
value zero). The instantaneous chance in the gradient, 𝑑𝐺, causes the current to increase in
proportion. At the same time, the resistance in the system causes the current to reduce in
proportion to the magnitude of the current. The predicted gradient is then given as the input
gradient minus the eddy-current contributions, which is the sum of their currents weighted by
their respective amplitude parameter.

This can be written in simplified form as:
𝑑𝐺𝑡 = 𝐺𝑡 − 𝐺𝑡−1
𝐼𝑛,𝑡 = 𝐼𝑛,𝑡−1 + 𝑑𝐺𝑡 − 𝑅𝑛 × 𝐼𝑛,𝑡−1
𝐺'𝑡 = 𝐺𝑡 − ∑ 𝐴𝑛 × 𝐼𝑛,𝑡

where:
• 𝐺𝑡 and 𝐺'𝑡 are the input and predicted waveforms at time point 𝑡 respectively
• 𝐼𝑛,𝑡 is the value of the current at time point 𝑡 for the nth eddy-current component
• 𝐴𝑛 and 𝑅𝑛 are the amplitude and rate constant parameters for the nth eddy-current
component.
• 𝑁 is the number of eddy-current components listed in the parameter file.
Computing the compensated time course
The compensation to be applied to the waveform can be computed using the following iterative
approach:

Given the desired waveform:
• Initialise the current estimate of the (compensated) input waveform as the desired
waveform
• Repeat for the desired number of iterations:
  o Compute the predicted waveform for the current input waveform
  o Compute the difference between the desired and predicted waveforms
  o Add the computed difference to the current input waveform, to produce the next
  estimate of the input waveform

#### Displaying the waveforms

The various waveforms can be displayed using the terminal_graphics library. 

#### Writing the final compensated time course

Your program should only store the output if provided with a third argument on the command-
line. If this is the case, you should write the final compensated time course to the file specified as
the last argument, in the same format as the original desired time course.

## Coursework 3

### Objective

To gain practical experience of C++ coding to design and implement a project to solve a more
complex problem.

### Introduction

Magnetic resonance imaging (MRI) is used clinically for the assessment of myocardial perfusion.
In this protocol, a contrast agent is injected intravenously into the patient and images of the heart
are acquired in each heartbeat over a period of 1-2 min to observe the first pass of the contrast
agent in the myocardium. This protocol therefore involves acquiring images of the heart at regular
time points, with a time interval of a few seconds. Any region in the myocardium with reduced
contrast uptake (reflecting a perfusion defect) can then be identified as having reduced signal
intensity compared to the rest of the myocardium during the first pass of the contrast agent in the
myocardium.

A research team is developing software to facilitate the analysis of cardiac MRI perfusion data.
This involves the calculation of different parameters related to the temporal evolution of the
contrast agent. The initial processing pipeline should implement the following functionalities:
1. Load one time series of cardiac MRI perfusion images from a series of PGM image files
(saved as “mri-01.pgm”, “mri-02.pgm”, etc).
2. Load the contrast agent information from a separate file called “contrast_info.txt”.
3. Create a 2D binary image (i.e. a mask, where pixels can only take values 0 or 1) of the same
size as the original image, where all pixels are set to 1 if inside an LVBP, or 0 otherwise.
The LVBP region should be defined as a 5x5 pixel square centred on the pixel at position
(x, y) = (74, 90) in the given example.
4. Calculate the average signal S(d) in the LVBP region for each time frame d, and plot this
timecourse on the terminal.
5. Identify the time frame of peak contrast concentration (dpeakBlood) in the LVBP region and
the corresponding mean LVBP signal (SpeakBlood) in the image at that time frame. Display
the image frame at peak contrast concentration in the terminal.
6. Identify the time frame of contrast arrival darrival in LVBP and the corresponding LVBP
signal (Sarrival) in the image at that time frame. This should be computed as the earliest
time frame (prior to dpeakBlood) at which the temporal gradient of S(d) (the signal within the
LVBP region) exceeds the threshold value of 10. The temporal gradient should be
computed as S(d+1)-S(d). Plot the gradient timecourse on the terminal.
7. Compute the temporal gradient (G) of the signal during contrast uptake as:
G = (SpeakBlood – Sarrival)/(dpeakBlood – darrival).
8. Display the contrast agent info associated with this scan as well as darrival, Sarrival, dpeakBlood,
SpeakBlood, and G.

### Instructions

Your task in this coursework is to write a C++ program which meets the requirements described
above.

A time series of images in plain PGM format is stored in the “data” folder. 

The contrast agent information is also stored in a separate file called “contrast_ info.txt”, which contains two lines: the first line contains the name of the contrast agent, the second line
represents the contrast dose used during the perfusion scan (a floating-point number in mmol/kg).

All indices in these instructions (time point and pixel coordinates) are expressed using zero-based
array indexing (i.e. starting as 0).

Note that a perfusion image dataset is a specific type of temporal dataset. In the future, the
research team would also like to model other types of temporal datasets, which may be acquired
in the absence of contrast agent but may have other additional properties. Given this wider
context, the functionality developed in this project should be designed to maximise its potential
for re-use, whether as part of this project or for the analysis of other types of images.
