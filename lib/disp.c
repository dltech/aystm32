
void sdInit()
{
    uint8_t initSeq[5] = { (uint8_t),
                           (uint8_t)
    }
    // сперва интерфейс
    i2c_set_7bit_addr_mode(DISPI2C);
    i2c_set_speed(DISPI2C,i2c_speed_fm_400k,SYSCLKHZ/1e6);
    // todo: blocking send seq
//             dma send seq
//             обработчик по окончанию (так же структура состояние отслеживать придется или по регистрам только удастся если да то круто)
}

void blockingSengSeq(uint8_t *seq)
{
    i2c_send_start(DISPI2C);
    i2c_send_data()
}

void fromDotMatrixToData()
{
    for(int i=0 ; i<BUFFER_SIZE)
}

void flashlight()
{

}
