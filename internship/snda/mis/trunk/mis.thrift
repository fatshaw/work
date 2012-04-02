struct MISRequest {
  1: string record_id,
  2: string user_id,
  3: string ip,
  4: string channel,
  5: string content
}

struct MISResponse {
  1: byte result,
  2: string reason
}

service MISInterface {
  MISResponse check(1: MISRequest request),
  void add2Q(1: MISRequest request, 2: string reason),
  byte ping(),
  void reload()
}
