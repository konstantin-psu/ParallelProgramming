//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// Task queue class
// ----------------
//

class Queue {
    var head: Task;   	// head pointer
    var tail: Task;   	// tail pointer
    var limit: int;      	// queue capacity (0 means unlimited)
    var length: int; 	// number of tasks in queue

    class Task {
        var val: int;
        var next: Task;
    }

    // Create a new task
    //
    proc create_task(val: int) {
        return new Task(val, nil);
    } 

    // Add a task to the tail of the queue
    // - If successful, return the queue's new length;
    //   otherwise, return 0.
    //
    proc add(task: Task) {
        if (limit <= 0 || length < limit) {
            if (tail==nil) {
                head = task;
                tail = task;
            } else {
                tail.next = task;
                tail = task;
            }
            length = length + 1;
            return length;
        }
        return 0;
    }

    // Remove a task from the head of the queue
    // - If no task in the queue, return nil.
    //
    proc remove() {
        var task: Task = nil;
        if (length > 0) {
            task = head;
            if (head == tail) {
                head = nil;
                tail = nil;
            } else {
                head = head.next;
            }
            length = length - 1;
        }
        return task;
    }
}

proc main() {
    var queue = new Queue(limit=0);
    for i in 1..3 do {
        queue.add(queue.create_task(i));
        writeln(queue);
    }
    for i in 1..3 do {
        queue.remove();
        writeln(queue);
    }

    // add code here ...
}
