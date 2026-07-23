-----------The Code Document of DMM6500-----------

--> Used QMH architecture.
--> The cases included are:
	* Initialize - This case initializes the connection for the device, and also initializes any queue or notifier is used.
	* Configure  - This case commences of functionality configuration like measurement function setting , channel name, buffer declaration etc.
	* Measure    - This case is the measurement case which commences of the following VIs.:
			> Fetch(single) data
	* Close      - This VI closes the reference of the device.