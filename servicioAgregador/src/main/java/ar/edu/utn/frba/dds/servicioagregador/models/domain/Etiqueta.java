package ar.edu.utn.frba.dds.servicioagregador.models.domain;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class Etiqueta {
  private Long id;
  private String titulo;

  public Etiqueta(String titulo) {
    this.titulo = titulo;
  }
}
