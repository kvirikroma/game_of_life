%ifndef life_runner_include_common
%define life_runner_include_common


struc life_runner
    .min_neighbors_to_exist     resb 1
    .max_neighbors_to_exist     resb 1
    .min_neighbors_to_be_born   resb 1
    .max_neighbors_to_be_born   resb 1
    .neighbors_that_matter      resb 1
    .field resq 1
endstruc


%endif
