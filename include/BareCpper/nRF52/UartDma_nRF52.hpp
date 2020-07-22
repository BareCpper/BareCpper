
static const nrf_drv_uart_config_t _name = { \
        .pselrxd = _rx_pin,                          \
        .pseltxd = _tx_pin,                          \
        .pselrts = _rts_pin,                         \
        .pselcts = _cts_pin,                         \
        .hwfc = _flow_control,                       \
        .parity = _parity,                           \
        .baudrate = _baud_rate,                      \
        .interrupt_priority = _irq_prio,             \

    typedef enum
{
    NRF_QUEUE_MODE_OVERFLOW,        //!< If the queue is full, new element will overwrite the oldest.
    NRF_QUEUE_MODE_NO_OVERFLOW,     //!< If the queue is full, new element will not be accepted.
} nrf_queue_mode_t;

typedef struct
{
    volatile size_t front;          //!< Queue front index.
    volatile size_t back;           //!< Queue back index.
    size_t max_utilization;         //!< Maximum utilization of the queue.
} nrf_queue_cb_t;

typedef struct
{
    nrf_queue_cb_t* p_cb;              //!< Pointer to the instance control block.
    void* p_buffer;          //!< Pointer to the memory that is used as storage.
    size_t           size;              //!< Size of the queue.
    size_t           element_size;      //!< Size of one element.
    nrf_queue_mode_t mode;              //!< Mode of the queue.
} nrf_queue_t;

    static uint8_t  serial0_queues_rxq_nrf_queue_buffer[(_size)+1];     
    static nrf_queue_cb_t  serial0_queues_rxq_nrf_queue_cb;

    , ) 
    const nrf_queue_t  serial0_queues_rxq __attribute__((section(STRINGIFY(nrf_queue)))) __attribute__((used))
        = \
        {                                                                                \
        .p_cb = &serial0_queues_rxq_nrf_queue_cb, \
        .p_buffer = serial0_queues_rxq_nrf_queue_buffer, \
        .size = 32, \
        .element_size = sizeof(uint8_t), \
        .mode = NRF_QUEUE_MODE_NO_OVERFLOW, \
        }


    static const nrf_serial_queues_t serial0_queues = { \
         .p_rxq = &serial0_queues_rxq,                                                 \
        .p_txq = &serial0_queues_txq,                                                 \
    }

