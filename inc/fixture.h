// chunck_size is the size of the data to be provided to the tested function
extern const size_t chunk_size;
// number_measurements allows to set the number of measurements per pass,
//  note that currently the report awaits enough_measurements = 10000 measurements.
extern const size_t number_measurements;
// do_one_computation would typically be one or more passes of the functions
//  you're timing using the input in range chunk_size pointed by data   
extern uint8_t do_one_computation(uint8_t *data);
// init_dut provides a mean to perform some initializations at startup
extern void init_dut(void);
// prepare_inputs allows one to provide the input data. Dudect will try 
//  and find discrepancies between the distributions of the data 
//  labelled in class 0 and class 1.
extern void prepare_inputs(uint8_t *input_data, uint8_t *classes);
