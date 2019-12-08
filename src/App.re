open Revery;
open Revery.UI;
open Revery.UI.Components;

module LoginForm = {
  open Client;

  let%component make = () => {
    let%hook (id, setId) = Hooks.state("");
    let%hook (password, setPassword) = Hooks.state("");
    let%hook (token, setToken) = Hooks.state(None);

    let login = () => {
      let%lwt client =
        Client.make(~site="http://localhost:8065")
        |> login(~login_id=id, ~password);
      setToken(_ => client.token);
      Lwt.return();
    };

    <View>
      <Input value=id onChange={(value, _) => setId(_ => value)} />
      <Input
        value=password
        onChange={(value, _) => setPassword(_ => value)}
      />
      <Clickable onClick={() => login() |> ignore}>
        <View>
          <Padding padding=4>
            <Text
              style=Style.[
                fontFamily("Roboto-Regular.ttf"),
                fontSize(14),
                margin(16),
              ]
              text="Login!"
            />
          </Padding>
        </View>
      </Clickable>
    </View>;
  };
};

let init = app => {
  let _ = Revery.Log.listen((_, msg) => print_endline("LOG: " ++ msg));

  let win = App.createWindow(app, "Welcome to Revery!");

  let containerStyle =
    Style.[
      position(`Absolute),
      justifyContent(`Center),
      alignItems(`Center),
      bottom(0),
      top(0),
      left(0),
      right(0),
    ];

  let innerStyle = Style.[flexDirection(`Row), alignItems(`FlexEnd)];

  let element =
    <View style=containerStyle>
      <View style=innerStyle> <LoginForm /> </View>
    </View>;

  let _ = Revery_Lwt.startEventLoop();
  let _ = UI.start(win, element);
  ();
};

App.start(init);
