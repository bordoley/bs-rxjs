type t('a);

type observable('a) = t('a);

module type S = {
  type t;
  type a;

  let asObservable: t => observable(a);
};

module type S1 = {
  type t('a);

  let asObservable: t('a) => observable('a);
};

[@bs.val] [@bs.scope "Observable"] [@bs.module "rxjs"]
external create: ([@bs.uncurry] (RxSubscriber.t('a) => unit)) => t('a) =
  "create";

[@bs.val] [@bs.module "rxjs"]
external fromEventImpl:
  (
    Dom.eventTarget,
    string,
    {
      .
      "capture": bool,
      "passive": bool,
    }
  ) =>
  t(Dom.event) =
  "fromEvent";

let fromEvent = (~capture=false, ~passive=true, eventName, target) => {
  let options = {"capture": capture, "passive": passive};
  fromEventImpl(target, eventName, options);
};

[@bs.val] [@bs.module "rxjs"]
external ofArrayNative: array('a) => t('a) = "of";
let ofArray = (~scheduler=?, values) =>
  switch (scheduler) {
  | None => ofArrayNative(values)
  | Some(scheduler) =>
    create(subscriber => {
      let length = values |> Js.Array.length;

      let rec loop = (start, ~shouldYield) => {
        let index = ref(start);
        let yieldRequested = ref(false);

        while (index^ <= length && ! yieldRequested^) {
          if (index^ < length) {
            subscriber |> RxSubscriber.next(values[index^]);
          } else {
            subscriber |> RxSubscriber.complete;
          };
          yieldRequested := shouldYield();
          incr(index);
        };

        if (index^ <= length) {
          RxPriorityScheduler.Result.yield(loop(index^));
        } else {
          RxPriorityScheduler.Result.complete;
        };
      };

      let schedulerSubscription =
        scheduler |> RxPriorityScheduler.schedule(loop(0));
      subscriber
      |> RxSubscriber.asDisposable
      |> RxDisposable.addDisposable(schedulerSubscription)
      |> ignore;
    })
  };

let ofList = {
  let ofListScheduledSource = (scheduler, list, subscriber) => {
    let rec loop = (list, ~shouldYield) =>
      if (RxSubscriber.isDisposed(subscriber)) {
        RxPriorityScheduler.Result.complete;
      } else {
        switch (list) {
        | [hd, ...tail] =>
          subscriber |> RxSubscriber.next(hd);

          /* Keep pushing values until told to yield */
          if (shouldYield()) {
            RxPriorityScheduler.Result.yield(loop(tail));
          } else {
            loop(tail, ~shouldYield);
          };
        | [] =>
          subscriber |> RxSubscriber.complete;
          RxPriorityScheduler.Result.complete;
        };
      };

    let schedulerSubscription =
      scheduler |> RxPriorityScheduler.schedule(loop(list));

    subscriber
    |> RxSubscriber.asDisposable
    |> RxDisposable.addDisposable(schedulerSubscription)
    |> ignore;
  };

  let ofListSynchronous = (list, subscriber) => {
    let rec loop = list =>
      switch (list) {
      | [hd, ...tail] =>
        subscriber |> RxSubscriber.next(hd);
        loop(tail);
      | [] => subscriber |> RxSubscriber.complete
      };
    loop(list);
  };

  (~scheduler=?, list) =>
    switch (scheduler) {
    | Some(scheduler) => create(ofListScheduledSource(scheduler, list))
    | None => create(ofListSynchronous(list))
    };
};

[@bs.val] [@bs.module "rxjs"] external ofValueNative: 'a => t('a) = "of";
let ofValue = (~scheduler=?, value) =>
  switch (scheduler) {
  | None => ofValueNative(value)
  | Some(scheduler) =>
    create(subscriber => {
      let rec cb = (count, ~shouldYield) => {
        let count = ref(count);
        if (count^ === 0) {
          subscriber |> RxSubscriber.next(value);
          incr(count);
        } else {
          subscriber |> RxSubscriber.complete;
          incr(count);
        };

        let yieldRequested = shouldYield();

        if (count^ < 2 && yieldRequested) {
          RxPriorityScheduler.Result.yield(cb(count^));
        } else if (count^ < 2) {
          cb(count^, ~shouldYield);
        } else {
          RxPriorityScheduler.Result.complete;
        };
      };

      let schedulerSubscription =
        scheduler |> RxPriorityScheduler.schedule(cb(0));
      subscriber
      |> RxSubscriber.asDisposable
      |> RxDisposable.addDisposable(schedulerSubscription)
      |> ignore;
    })
  };

[@bs.send.pipe: t('a)]
external subscribeImpl:
  ((. unit) => unit, (. unit) => unit, (. unit) => unit) => RxDisposable.t =
  "subscribe";

let subscribe = observable =>
  observable |> subscribeImpl((.) => (), (.) => (), (.) => ());

[@bs.send] external toPromise: t('a) => Js.Promise.t('a) = "toPromise";