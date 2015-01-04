# Distributed CXXNET

This fold extends [cxxnet](https://github.com/antinucleon/cxxnet) into a
multiple-machine version.

## Implementation

In each minibatch, all workers push the gradients into the servers layer by
layer, next each server aggregates these gradients and update the layer weights,
and then workers pull the updated weights and move to next minibatch.

There are three ways to reduce the communication efficiency:

* `push` is asynchronous, so that a worker can start to compute the gradient of
   another layer without waiting the gradients of the previous layer has been
   sent successfully.
* `pull` allows delays. A worker may pull a slightly out-of-date model from the
   servers. the delay is upper bounded by `max_delay`
* A server may update the weights without waiting all workers. In other words,
  a server can ignore the straggler workers.

`sync.h` is the communication interface for cxxnet. `cxxnet_scheduler.h`,
`cxxnet_server.h` and `cxxnet_worker.h` decompose cxxnet into scheduler node,
server node, and worker node, respectively. `cxxnet.proto` has the configurations
and RPC call arguments.

## Build and run

First compile cxxnet with the flag `-DCXXNET_PS=1` to obtain all .o files. Then
compile the parameter server to obtain the binary. cxxnet can be complied with
low version gcc to satisfy the requirement of CUDA.

See `example/cxxnet/MNIST` for examples to run the system.

## TODO list

* data parallelism: the scheduler splits the data (usually multiple files) and
  send the according configuration to workers nodes.
* improve the synchronization interface of cxxnet
* the server node can parse the cxxnet configure to get all layers names, and
  call the update function of cxxnet. (now workers execute the update functions)
* cxxnet copies the parameters need for communication, which may be on GPU
  memory or stride, into a continuous main memory.
