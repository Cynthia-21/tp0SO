package ar.edu.utn.frba.dds.servicioagregador.models.domain;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class Categoria {
  Long id;
  private String nombre;

  public Categoria(String nombre) {
    this.nombre = nombre;
  }
}
