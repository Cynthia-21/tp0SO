package ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Hecho;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.solicitudes.SolicitudEliminacion;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.input.HechoInputDTO;
import ar.edu.utn.frba.dds.servicioagregador.models.mappers.HechoProxyMapper;
import lombok.Data;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.MediaType;
import org.springframework.stereotype.Component;
import org.springframework.web.reactive.function.client.WebClient;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

@Data
@Component
public class FuenteProxy implements Fuente {
  private String tipo;
  private String proxy;
  private final WebClient webClient;
  @Value("${url.fuente.proxy}")
  private String baseUrl;
  @Autowired
  private HechoProxyMapper mapper;

  public FuenteProxy() {
    this.tipo = "proxy";
    this.webClient = WebClient.builder().baseUrl(this.baseUrl).build();
  }

  public FuenteProxy(String proxy) {
    this.tipo = "proxy";
    this.proxy = proxy;
    this.webClient = WebClient.builder().baseUrl(this.baseUrl).build();
  }

  @Override
  public List<Hecho> importarHechos() {
    return webClient
        .get()
        .uri("/{proxyId}/hechos", this.proxy)
        .retrieve()
        .bodyToFlux(HechoInputDTO.class)
        .map(hechoDto -> this.mapper.mapearHecho(hechoDto))
        .collectList()
        .block();
  }

  @Override
  public List<Hecho> importarHechosRecientes() {
    return List.of();
  }

  @Override
  public void eliminarHecho(Hecho hecho) {
    Map<String, Object> body = new HashMap<>();
    body.put("id", hecho.getId());
    body.put("titulo", hecho.getTitulo());

    webClient.post()
        .uri("/hechosEliminados")
        .contentType(MediaType.APPLICATION_JSON)
        .bodyValue(body)
        .retrieve()
        .toBodilessEntity()
        .block();
    }
}
