package ar.edu.utn.frba.dds.servicioagregador.models.domain.fuentes;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Hecho;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.solicitudes.SolicitudEliminacion;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.input.HechoInputDTO;
import ar.edu.utn.frba.dds.servicioagregador.models.mappers.HechoEstaticoMapper;
import lombok.Getter;
import lombok.Setter;
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

@Getter
@Setter
@Component
public class FuenteEstatica implements Fuente {
  private String tipo;
  private Archivo archivo;
  private final WebClient webClient;
  @Value("${url.fuente.estatica}")
  private String baseUrl;
  @Autowired
  private HechoEstaticoMapper mapper;

  public FuenteEstatica() {
    this.tipo = "estatica";
    this.webClient = WebClient.builder().baseUrl(this.baseUrl).build();
  }

  public FuenteEstatica(Archivo archivo) {
    this.tipo = "estatica";
    this.archivo = archivo;
    this.webClient = WebClient.builder().baseUrl(this.baseUrl).build();
  }

  public boolean soporta(Hecho hecho) {
    return hecho.getArchivoOrigen() != null;
  }

  @Override
  public List<Hecho> importarHechos() {
    if (this.archivo != null) {
      return webClient
          .get()
          .uri("/{archivo}/hechos", this.archivo.getNombre())
          .retrieve()
          .bodyToFlux(HechoInputDTO.class)
          .map(hechoDto -> this.mapper.mapearHecho(hechoDto))
          .collectList()
          .block();
    } else {
      return webClient
          .get()
          .uri("/hechos")
          .retrieve()
          .bodyToFlux(HechoInputDTO.class)
          .map(hechoDto -> this.mapper.mapearHecho(hechoDto))
          .collectList()
          .block();
    }
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
