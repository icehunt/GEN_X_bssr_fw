 # How to use this driver
1. Initialize the FDCAN peripheral using HAL_FDCAN_Init function.
2. If needed , configure the reception filters and optional features using the following configuration functions:
• HAL_FDCAN_ConfigClockCalibration : Configure the FDCAN clock calibration unit
• HAL_FDCAN_GetClockCalibrationState : Get the clock calibration state
• HAL_FDCAN_ResetClockCalibrationState : Reset the clock calibration state
• HAL_FDCAN_GetClockCalibrationCounter : Get the clock calibration counters values
• HAL_FDCAN_ConfigFilter : Configure the FDCAN reception filters
• HAL_FDCAN_ConfigGlobalFilter : Configure the FDCAN global filter
• HAL_FDCAN_ConfigExtendedIdMask : Configure the extended ID mask
• HAL_FDCAN_ConfigRxFifoOverwrite : Configure the Rx FIFO operation mode
• HAL_FDCAN_ConfigFifoWatermark : Configure the FIFO watermark
• HAL_FDCAN_ConfigRamWatchdog : Configure the RAM watchdog
• HAL_FDCAN_ConfigTimestampCounter : Configure the timestamp counter
• HAL_FDCAN_EnableTimestampCounter : Enable the timestamp counter
• HAL_FDCAN_DisableTimestampCounter : Disable the timestamp counter
• HAL_FDCAN_GetTimestampCounter : Get the timestamp counter value
• HAL_FDCAN_ResetTimestampCounter : Reset the timestamp counter to zero
• HAL_FDCAN_ConfigTimeoutCounter : Configure the timeout counter
• HAL_FDCAN_EnableTimeoutCounter : Enable the timeout counter
• HAL_FDCAN_DisableTimeoutCounter : Disable the timeout counter
• HAL_FDCAN_GetTimeoutCounter : Get the timeout counter value
• HAL_FDCAN_ResetTimeoutCounter : Reset the timeout counter to its start value
• HAL_FDCAN_ConfigTxDelayCompensation : Configure the transmitter delay compensation
• HAL_FDCAN_EnableTxDelayCompensation : Enable the transmitter delay compensation
• HAL_FDCAN_DisableTxDelayCompensation : Disable the transmitter delay compensation
3. Start the FDCAN module using HAL_FDCAN_Start function. At this level the node is active on the bus: it can
send and receive messages.
• HAL_FDCAN_Start : Start the FDCAN module
• HAL_FDCAN_Stop : Stop the FDCAN module and enable access to configuration registers
• HAL_FDCAN_AddMessageToTxFifoQ : Add a message to the Tx FIFO/Queue and activate the
corresponding transmission request
• HAL_FDCAN_AddMessageToTxBuffer : Add a message to a dedicated Tx buffer
• HAL_FDCAN_EnableTxBufferRequest : Enable transmission request
• HAL_FDCAN_AbortTxRequest : Abort transmission request
• HAL_FDCAN_GetRxMessage : Get an FDCAN frame from the Rx Buffer/FIFO zone into the message RAM
• HAL_FDCAN_GetTxEvent : Get an FDCAN Tx event from the Tx Event FIFO zone into the message RAM
• HAL_FDCAN_GetHighPriorityMessageStatus : Get high priority message status
• HAL_FDCAN_GetProtocolStatus : Get protocol status
• HAL_FDCAN_GetErrorCounters : Get error counter values
• HAL_FDCAN_IsRxBufferMessageAvailable : Check if a new message is received in the selected Rx buffer
• HAL_FDCAN_IsTxBufferMessagePending : Check if a transmission request is pending on the selected Tx
buffer
• HAL_FDCAN_GetRxFifoFillLevel : Return Rx FIFO fill level
• HAL_FDCAN_GetTxFifoFreeLevel : Return Tx FIFO free level
• HAL_FDCAN_IsRestrictedOperationMode : Check if the FDCAN peripheral entered Restricted Operation
Mode
• HAL_FDCAN_ExitRestrictedOperationMode : Exit Restricted Operation Mode
4. The following Tx control functions can only be called when the FDCAN module is started:
- HAL_FDCAN_AddMessageToTxFifoQ
- HAL_FDCAN_EnableTxBufferRequest
- HAL_FDCAN_AbortTxRequest
5. When a message is received into the FDCAN message RAM, it can be retrieved using the
HAL_FDCAN_GetRxMessage function.
6. Calling the HAL_FDCAN_Stop function stops the FDCAN module by entering it to initialization mode and reenabling access to configuration registers through the configuration functions listed here above.
7. All other control functions can be called any time after initialization phase, no matter if the FDCAN module is
started or stoped.
Polling mode operation
1. Reception and transmission states can be monitored via the following functions:
- HAL_FDCAN_IsRxBufferMessageAvailable
- HAL_FDCAN_IsTxBufferMessagePending
- HAL_FDCAN_GetRxFifoFillLevel
- HAL_FDCAN_GetTxFifoFreeLevel
Interrupt mode operation
1. There are two interrupt lines: line 0 and 1. By default, all interrupts are assigned to line 0. Interrupt lines can
be configured using HAL_FDCAN_ConfigInterruptLines function.
2. Notifications are activated using HAL_FDCAN_ActivateNotification function. Then, the process can be
controlled through one of the available user callbacks: HAL_FDCAN_xxxCallback.
• HAL_FDCAN_ClockCalibrationCallback
• HAL_FDCAN_TxEventFifoCallback
• HAL_FDCAN_RxFifo0Callback
• HAL_FDCAN_RxFifo1Callback
• HAL_FDCAN_TxFifoEmptyCallback
• HAL_FDCAN_TxBufferCompleteCallback
• HAL_FDCAN_TxBufferAbortCallback
• HAL_FDCAN_RxBufferNewMessageCallback
• HAL_FDCAN_HighPriorityMessageCallback
• HAL_FDCAN_TimestampWraparoundCallback
• HAL_FDCAN_TimeoutOccurredCallback
• HAL_FDCAN_ErrorCallback
• HAL_FDCAN_TTSchedSyncCallback
• HAL_FDCAN_TTTimeMarkCallback
• HAL_FDCAN_TTStopWatchCallback
• HAL_FDCAN_TTGlobalTimeCallback