package ar.edu.utn.frba.dds.servicioagregador.models.domain.contenidoMultimedia;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.NoArgsConstructor;
import lombok.Setter;

@Setter
@AllArgsConstructor
@NoArgsConstructor
@Builder
public class ContenidoMultimedia {
  String path;
  String descripcion;
  TipoContenido tipoContenido;

  public ContenidoMultimedia(String descripcion) {
    this.path = "";
    this.descripcion = descripcion;
    this.tipoContenido = null;
  }
}