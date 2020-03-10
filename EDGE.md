# Edge computing architectures

## Prometeo currently:

* Gathers temperature, humidity, and smoke data and sends that data to the IBM Cloud for processing in real-time.
* It depends on either a Wi-Fi hotspot carried by the firefighter, or, in a Project OWL integration prototype, LoRa.
* No data is cached or stored on the devices themselves.

## Shifted edge approach

One potential architecture that is being explored is to emit all data to a device at the control center. Something that is within LoRa range of all devices, or otherwise on a somewhat local network.

In this case, data could be cached, stored, and analyzed before being sent onward to the IBM Cloud.

## True edge approach

Another potential architecture is to upgrade the Arduino based-boards to be a Raspberry Pi or other full-blown Linux system with compute and storage capabilities. 

This could support offline use cases, and perform edge analytics on data before sending high-priority alerts back to the cloud. 

This approach could also better support over-the-air (OTA) updates and mobile-device management (MDM).

## Device to smartphone approach

Another approach is to keep the devices very simple, and instead pair them with a native applicaiton on the Android or iPhone device that the firefighter carries.

So, instead of applying additional engineering to custom devices, the approach could leverage capabilities native to the SDKs of these smartphones when tethered.

