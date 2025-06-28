package ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Hecho;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.solicitudes.SolicitudEliminacion;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.input.HechoInputDTO;
import ar.edu.utn.frba.dds.servicioagregador.models.mappers.HechoDinamicoMapper;
import lombok.Data;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.MediaType;
import org.springframework.stereotype.Component;
import org.springframework.web.reactive.function.client.WebClient;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@Data
@Component
public class FuenteDinamica implements Fuente {
  private String tipo;
  private final WebClient webClient;
  @Value("${url.fuente.dinamica}")
  private String baseUrl;
  @Autowired
  private HechoDinamicoMapper mapper;

  public FuenteDinamica() {
    this.tipo = "dinamica";
    this.webClient = WebClient.builder().baseUrl(this.baseUrl).build();
  }

  @Override
  public List<Hecho> importarHechos() {
    return webClient
        .get()
        .uri("/hechos")
        .retrieve()
        .bodyToFlux(HechoInputDTO.class)
        .map(hechoDto -> this.mapper.mapearHecho(hechoDto))
        .collectList()
        .block();
  }

  @Override
  public List<Hecho> importarHechosRecientes() {
    LocalDateTime fechaAlta = LocalDateTime.now().minusHours(1);
    String fechaAltaStr = fechaAlta.format(DateTimeFormatter.ISO_LOCAL_DATE_TIME); //año-mes-diaThora
    String path = this.baseUrl + "/hechos";

    List<Hecho> hechos = webClient.get()
        .uri(uriBuilder -> uriBuilder
            .path(path)
            .queryParam("fechaAlta", fechaAltaStr)
            .build())
        .retrieve()
        .bodyToFlux(HechoInputDTO.class)
        .map(hechoDto -> this.mapper.mapearHecho(hechoDto))
        .collectList()
        .block();
    return hechos;
  }

  @Override
  public void eliminarHecho(Hecho hecho) {
    Map<String, Object> body = new HashMap<>();
    body.put("id", hecho.getId());
    body.put("estaEliminado", true);

    webClient.patch()
        .uri("/hechos")
        .contentType(MediaType.APPLICATION_JSON)
        .bodyValue(body) // Como es un patch, veo mas optimo mandar el id y el estaEliminado en true de una
        .retrieve()
        .toBodilessEntity()
        .block();
  }
}
