#include "backsubstitute.h"

void * update_state_using_previous_layers(void *args){
	nn_thread_t * data = (nn_thread_t *)args;
	elina_manager_t * man = data->man;
	fppoly_t *fp = data->fp;
	fppoly_internal_t * pr = fppoly_init_from_manager(man, ELINA_FUNID_ASSIGN_LINEXPR_ARRAY);
	size_t layerno = data->layerno;
	size_t idx_start = data->start;
	size_t idx_end = data->end;
	size_t i;
	int k;
	
	neuron_t ** out_neurons = fp->layers[layerno]->neurons;
	size_t num_out_neurons = fp->layers[layerno]->dims;
	
	for(i=idx_start; i < idx_end; i++){
		bool already_computed= false;
		expr_t *lexpr = copy_expr(out_neurons[i]->lexpr);
		expr_t *uexpr = copy_expr(out_neurons[i]->uexpr);
		out_neurons[i]->lb = get_lb_using_previous_layers(man, fp, lexpr, layerno);
		out_neurons[i]->ub = get_ub_using_previous_layers(man, fp, uexpr, layerno);
		free_expr(lexpr);
		free_expr(uexpr);
		
	}
	return NULL;
}


void update_state_using_previous_layers_parallel(elina_manager_t *man, fppoly_t *fp, size_t layerno){
	//size_t NUM_THREADS = get_nprocs();
    size_t NUM_THREADS = 1;
	nn_thread_t args[NUM_THREADS];
	size_t num_out_neurons = fp->layers[layerno]->dims;
	size_t i = 0;
	
    size_t idx_start = 0;
    size_t idx_n = num_out_neurons / NUM_THREADS;
    size_t idx_end = idx_start + idx_n;
		
    args[i].start = idx_start; 
    args[i].end = idx_end;   
    args[i].man = man;
    args[i].fp = fp;
    args[i].layerno = layerno;
    args[i].linexpr0 = NULL;
    args[i].res = NULL;
    update_state_using_previous_layers((void*)&args[i]);
			
}
