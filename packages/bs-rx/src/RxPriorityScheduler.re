type t = int;

type callbackHandle;
type fn;
type callback = unit => fn;

[@bs.val] [@bs.module "scheduler"]
external runWithPriority: (int, unit => 't) => 't = "unstable_runWithPriority";

[@bs.val] [@bs.module "scheduler"]
external schedulerScheduleCallback: callback => callbackHandle =
  "unstable_scheduleCallback";

[@bs.val] [@bs.module "scheduler"]
external schedulerCancelCallback: callbackHandle => unit =
  "unstable_cancelCallback";

[@bs.val] [@bs.module "scheduler"]
external schedulerShouldYield: unit => bool = "unstable_shouldYield";

type continuation = (~shouldYield: unit => bool) => result

and result =
  | Yield(continuation)
  | Complete;

module Continuation = {
  type t = continuation;
};

module Result = {
  type t = result = | Yield(continuation) | Complete;

  let yield = continuation => Yield(continuation);
  let complete = Complete;
};

let schedule = (continuation, scheduler) => {
  let disposable = RxDisposable.create();

  let shouldYield = () => {
    let isDisposed = RxDisposable.isDisposed(disposable);
    isDisposed || schedulerShouldYield();
  };

  let callback = () => {
    let isDisposed = RxDisposable.isDisposed(disposable);

    if (!isDisposed) {
      let result = continuation(~shouldYield);
      switch (result) {
      | Yield(continuation) => Some(continuation)
      | Complete => None
      };
    } else {
      None;
    };
  };

  let priority = scheduler;
  let callbackHandle =
    runWithPriority(priority, () =>
      schedulerScheduleCallback(Obj.magic(callback))
    );

  disposable
  |> RxDisposable.addTeardown(() => schedulerCancelCallback(callbackHandle));
};

let immediate = 1;
let userBlocking = 2;
let normal = 3;
let low = 4;
let idle = 5;