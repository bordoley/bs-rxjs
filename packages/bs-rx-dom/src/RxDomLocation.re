type history;
[@bs.val] [@bs.scope "window"] external history: history = "history";
[@bs.send.pipe: history]
external pushState: (option(unit), string, string) => unit = "";

type location;
[@bs.val] [@bs.scope "window"] external location: location = "location";
[@bs.get] external href: location => string = "";

[@bs.val] external window: Dom.eventTarget = "window";

type t = unit;

let instance: t = ();

let observable =
  window
  |> RxObservable.fromEvent(~passive=true, "popstate")
  |> RxObservables.map(_ => location |> href)
  |> RxObservables.startWith(location |> href)
  |> RxObservables.shareReplayLast;

let asObservable = _ => observable;

 let notify = (prevValue, nextValue) => {
   if (prevValue !== nextValue) {
     history |> pushState(None, "", nextValue);
   };
 }

let update = (f, _) => {
  let prevValue = location |> href;
  let nextValue = f(prevValue);
  notify(prevValue, nextValue);
};

let update1 = (f, ctx0, _) => {
  let prevValue = location |> href;
  let nextValue = f(ctx0, prevValue);
  notify(prevValue, nextValue);
};

let update2 = (f, ctx0, ctx1, _) => {
  let prevValue = location |> href;
  let nextValue = f(ctx0, ctx1, prevValue);
  notify(prevValue, nextValue);
};

let update3 = (f, ctx0, ctx1, ctx2, _) => {
  let prevValue = location |> href;
  let nextValue = f(ctx0, ctx1, ctx2, prevValue);
  notify(prevValue, nextValue);
};

let update4 = (f, ctx0, ctx1, ctx2, ctx3, _) => {
  let prevValue = location |> href;
  let nextValue = f(ctx0, ctx1, ctx2, ctx3, prevValue);
  notify(prevValue, nextValue);
};

let update5 = (f, ctx0, ctx1, ctx2, ctx3, ctx4, _) => {
  let prevValue = location |> href;
  let nextValue = f(ctx0, ctx1, ctx2, ctx3, ctx4, prevValue);
  notify(prevValue, nextValue);
};