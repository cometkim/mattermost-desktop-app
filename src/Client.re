open Cohttp;
open Cohttp_lwt_unix;

type config = {site: string};

type t = {
  config,
  token: option(string),
};

module URI = {
  let base_url = (client: t) => client.config.site ++ "/api/v4";

  let login = (client: t) =>
    client |> base_url |> Printf.sprintf("%s/users/login") |> Uri.of_string;

  let users = (client: t) =>
    client |> base_url |> Printf.sprintf("%s/users") |> Uri.of_string;

  let user = (~userId: string, client: t) =>
    client
    |> base_url
    |> Printf.sprintf("%s/users/%s", _, userId)
    |> Uri.of_string;
};

let make = (~site: string): t => {
  let config: config = {site: site};

  {config, token: None};
};

[@deriving to_yojson]
type auth_payload = {
  login_id: string,
  password: string,
};

let login = (~login_id: string, ~password: string, client: t) => {
  let headers = Header.init_with("Content-Type", "application/json");
  let body =
    auth_payload_to_yojson({login_id, password})
    |> Yojson.Safe.to_string
    |> Cohttp_lwt.Body.of_string;

  let%lwt (response, payload) =
    client |> URI.login |> Client.post(~headers, ~body);

  let token = Header.get(response.headers, "Token");

  Lwt.return({...client, token});
};

let with_token = (token: string, client: t) => {
  {...client, token: Some(token)};
};
