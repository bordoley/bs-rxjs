module State = {
  type t = {
    count: int,
    greeting: string,
    show: bool,
  };
};

module Action = {
  type t =
    | Click
    | SetGreeting(string)
    | Toggle;
};

let greetingStateComponent = {
  let useMemoCb =
    React.useMemo3((dispatcher, dispatch, action, ()) =>
      dispatcher |> dispatch(action)
    );

  let name = "GreetingStateComponent";

  let render = (~key as _=?, ~props, ()) => {
    let {
      RxReactStateDispatchComponent.RenderProps.state: {
        State.count,
        greeting,
        show,
      },
      dispatch,
      dispatcher,
    } = props;

    let incrementCount = useMemoCb(dispatcher, dispatch, Action.Click);
    let toggle = useMemoCb(dispatcher, dispatch, Action.Toggle);

    Components.greeting(
      ~props={count, greeting, show, incrementCount, toggle},
      (),
    );
  };

  RxReactStateDispatchComponent.create(~name, ~render, ());
};

let dispatcher: RxEvent.t(Action.t) = RxEvent.create();

let stateStore =
  RxValue.create(
    {count: 0, greeting: "Initial Greeting", show: false}: State.t,
  );

let sideEffectsSubscription = {
  let onNext = action => {
    let reducer = state =>
      switch (action) {
      | Action.Click => {...state, State.count: state.State.count + 1}
      | Action.SetGreeting(greeting) => {...state, State.greeting}
      | Action.Toggle => {...state, State.show: !state.State.show}
      };

    stateStore |> RxValue.update(reducer);
  };

  let onComplete = () => stateStore |> RxValue.dispose;

  dispatcher
  |> RxEvent.asObservable
  |> RxObservables.observe(~onNext, ~onError=_ => (), ~onComplete)
  |> RxObservable.subscribe;
};

ReactDom.renderToElementWithId(
  greetingStateComponent(
    ~props={
      dispatch: RxEvent.dispatch,
      stateStream: stateStore |> RxValue.asObservable,
      dispatcher,
    },
    (),
  ),
  "index2",
);

let state = ref(true);
Js.Global.setInterval(
  () => {
    Js.log("settting props");
    state := ! state^;
    dispatcher
    |> RxEvent.dispatch(Action.SetGreeting(state^ ? "true" : "false"));
  },
  5000,
)
|> ignore;

RxDomLocation.instance
|> RxDomLocation.asObservable
|> RxObservables.onNext(Js.log)
|> RxObservable.subscribe;

RxDomLocation.instance |> RxDomLocation.update(_ => "?next=rock");