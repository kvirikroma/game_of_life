%ifndef life_runner_include_common
%define life_runner_include_common


struc life_runner
    .field                      resq 1
    .min_neighbors_to_exist     resb 1
    .max_neighbors_to_exist     resb 1
    .min_neighbors_to_be_born   resb 1
    .max_neighbors_to_be_born   resb 1
    .neighbors_that_matter      resb 1
    .disable_cyclic_adressing   resb 1
endstruc

LIFE_RUNNER_SIZE equ 14


%endif
