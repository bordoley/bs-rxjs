module Props = {
  type t('state, 'action, 'dispatcher) = {
    stateStream: RxObservable.t('state),
    dispatch: ('action, 'dispatcher) => unit,
    dispatcher: 'dispatcher,
  };
};

module RenderProps = {
  type t('state, 'action, 'dispatcher) = {
    state: 'state,
    dispatch: ('action, 'dispatcher) => unit,
    dispatcher: 'dispatcher,
  };
};

let createReactComponent = {
  let f =
      (
        {stateStream, dispatch, dispatcher}:
          Props.t('state, 'action, 'dispatcher),
      ) => {
    let stateToRenderProps =
        (state): RenderProps.t('state, 'action, 'dispatch) => {
      state,
      dispatch,
      dispatcher,
    };
    
    stateStream |> RxObservables.map(stateToRenderProps);
  };

  (~name=?, ~renderDefault=?, ~renderExn=?, ~render) => {
    let propsToState = RxObservables.switchMap(f);

    BsRxReact.createReactComponent(
      ~name?,
      ~propsToState,
      ~renderDefault?,
      ~renderExn?,
      ~render,
    );
  };
};

let create = (~name=?, ~renderDefault=?, ~renderExn=?, ~render, ()) => {
  let statefulComponent =
    createReactComponent(~name?, ~renderDefault?, ~renderExn?, ~render);
  React.Element.create(statefulComponent);
};